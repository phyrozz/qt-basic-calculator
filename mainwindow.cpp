#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QJSEngine"
#include "QRegularExpression"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->calcField->setAlignment(Qt::AlignRight);
    this->setWindowTitle("Basic Calculator");

    // Add css properties to widgets
    this->setStyleSheet("QTextEdit { background-color: #444444; color: white; border: none; border-radius: 8px; } "
                        "QPushButton { background-color: #222222; color: white; border: none; border-radius: 8px; min-width: 80px; font-size: 24px; } "
                        "QPushButton:hover { background-color: #333333; } "
                        "QMainWindow { background-color: #111111; }");

    // Connect number buttons to the slot
    connect(ui->zeroBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->oneBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->twoBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->threeBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->fourBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->fiveBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->sixBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->sevenBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->eightBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));
    connect(ui->nineBtn, SIGNAL(clicked()), this, SLOT(onNumberButtonClicked()));

    // Connect operation buttons to the slot
    connect(ui->addBtn, SIGNAL(clicked()), this, SLOT(onOperationButtonClicked()));
    connect(ui->subtractBtn, SIGNAL(clicked()), this, SLOT(onOperationButtonClicked()));
    connect(ui->multiplyBtn, SIGNAL(clicked()), this, SLOT(onOperationButtonClicked()));
    connect(ui->divideBtn, SIGNAL(clicked()), this, SLOT(onOperationButtonClicked()));

    // Connect other buttons to the slot
    connect(ui->equalsBtn, SIGNAL(clicked()), this, SLOT(onEqualsButtonClicked()));
    connect(ui->clearBtn, SIGNAL(clicked()), this, SLOT(onClearButtonClicked()));
    connect(ui->signedBtn, SIGNAL(clicked()), this, SLOT(onSignedButtonClicked()));
    connect(ui->decimalBtn, SIGNAL(clicked()), this, SLOT(onDecimalButtonClicked()));
    connect(ui->percentBtn, SIGNAL(clicked()), this, SLOT(onPercentButtonClicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onNumberButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString currentText = ui->calcField->toPlainText();
        QString btnValue = button->text();
        if (btnValue == "0") {
            if (!currentText.isEmpty()) {
                currentText += button->text();
                ui->calcField->setPlainText(currentText);
            }
        } else {
            currentText += button->text();
            ui->calcField->setPlainText(currentText);
        }
    }
}

void MainWindow::onOperationButtonClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString currentText = ui->calcField->toPlainText();
    if (button && !currentText.isEmpty()) {
        currentText += " " + button->text() + " ";
        ui->calcField->setPlainText(currentText);
    }
}

void MainWindow::onEqualsButtonClicked()
{
    QString expressionToString = ui->calcField->toPlainText();
    QJSEngine expression;
    double result = expression.evaluate(expressionToString).toNumber();

    QString currentText = ui->calcField->toPlainText();
    if (!currentText.isEmpty()) {
        if (std::isinf(result)) {
            // Handle divide by zero error
            ui->calcField->setPlainText("Divide by zero error!");
            // Disable other buttons except "CLS"
            disableAllButtonsExceptClear();
        } else {
            // Convert the result to string and update the calcField
            ui->calcField->setPlainText(QString::number(result));
        }
    }
}

void MainWindow::onClearButtonClicked()
{
    // Clear the text field
    ui->calcField->setPlainText("");

    // Re-enable all buttons
    enableAllButtons();
}

void MainWindow::onSignedButtonClicked()
{
    QString currentText = ui->calcField->toPlainText().trimmed();

    if (!currentText.isEmpty()) {
        // Check if the current text is a mathematical expression using regular expressions
        QRegularExpression expressionRegex("[+\\-*/^%]");
        if (currentText.contains(expressionRegex)) {
            // Enclose the expression with parentheses and add a negative sign
            currentText = QString("(-(%1))").arg(currentText);
        } else {
            // Add a negative sign at the beginning
            currentText.prepend('-');
        }

        // Update the calcField with the modified text
        ui->calcField->setPlainText(currentText);
    }
}

void MainWindow::onDecimalButtonClicked()
{
    QString currentText = ui->calcField->toPlainText().trimmed();

    if (!currentText.isEmpty()) {
        currentText.append('.');
    } else {
        currentText.append("0.");
    }

    ui->calcField->setPlainText(currentText);
}

void MainWindow::onPercentButtonClicked()
{
    QString expression = ui->calcField->toPlainText().trimmed();

    // Check if the expression is not empty
    if (!expression.isEmpty()) {
        // Regular expression to match the last number in the expression
        QRegularExpression regex("(\\d+(\\.\\d+)?)$");
        QRegularExpressionMatch match = regex.match(expression);

        if (match.hasMatch()) {
            QString lastNumberStr = match.captured(1);
            double lastNumber = lastNumberStr.toDouble();

            // Calculate the percentage (0.01 times the number)
            double percentage = lastNumber * 0.01;

            // Replace the last number in the expression with the calculated percentage
            expression.replace(regex, QString::number(percentage));

            // Update the calcField with the modified expression
            ui->calcField->setPlainText(expression);
        }
    }
}

// Disable all buttons except CLS when error appears on the field
void MainWindow::disableAllButtonsExceptClear()
{
    foreach (QPushButton *button, findChildren<QPushButton*>()) {
        if (button->objectName() != "clearBtn") {
            button->setDisabled(true);
        }
    }
}

// Enable all buttons when CLS is clicked in the case of an error shown on the field
void MainWindow::enableAllButtons()
{
    foreach (QPushButton *button, findChildren<QPushButton*>()) {
        button->setEnabled(true);
    }
}
