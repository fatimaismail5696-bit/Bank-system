#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// ===== Transaction Class =====
class Transaction {
public:
    string type;       
    double amount;
    string dateTime;
    string note;

    Transaction(string t, double amt, string n = "") {
        type = t;
        amount = amt;
        note = n;

        // Get current date & time in old-compatible way
        time_t now = time(0);
        char* dt = ctime(&now);
        dateTime = string(dt);
        if (!dateTime.empty() && dateTime[dateTime.size() - 1] == '\n')
            dateTime.erase(dateTime.size() - 1); // remove newline
    }

    void display() const {
        cout << dateTime << " | " << type << " | Amount: " << amount;
        if (!note.empty()) cout << " | " << note;
        cout << endl;
    }
};

// ===== Account Class =====
class Account {
private:
    double balance;
    vector<Transaction> history;

public:
    string accountNumber;

    Account(string accNo, double initialBalance = 0.0) {
        accountNumber = accNo;
        balance = initialBalance;
    }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount.\n";
            return;
        }
        balance += amount;
        history.push_back(Transaction("Deposit", amount));
        cout << "Deposit successful. New Balance: " << balance << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount.\n";
            return;
        }
        if (amount > balance) {
            cout << "Insufficient funds.\n";
            return;
        }
        balance -= amount;
        history.push_back(Transaction("Withdrawal", amount));
        cout << "Withdrawal successful. New Balance: " << balance << endl;
    }

    void transfer(Account &toAccount, double amount) {
        if (amount <= 0) {
            cout << "Invalid transfer amount.\n";
            return;
        }
        if (amount > balance) {
            cout << "Insufficient funds for transfer.\n";
            return;
        }
        balance -= amount;
        toAccount.balance += amount;
        history.push_back(Transaction("Transfer Out", amount, "To: " + toAccount.accountNumber));
        toAccount.history.push_back(Transaction("Transfer In", amount, "From: " + accountNumber));
        cout << "Transfer successful. New Balance: " << balance << endl;
    }

    void showBalance() const {
        cout << "Account: " << accountNumber << " | Balance: " << balance << endl;
    }

    void showTransactions() const {
        cout << "\n--- Transaction History for " << accountNumber << " ---\n";
        for (size_t i = 0; i < history.size(); i++) {
            history[i].display();
        }
    }
};

// ===== Customer Class =====
class Customer {
public:
    string name;
    string customerID;
    Account account;

    Customer(string n, string id, string accNo, double initialBalance = 0.0)
        : name(n), customerID(id), account(accNo, initialBalance) {}

    void displayInfo() const {
        cout << "Customer: " << name << " | ID: " << customerID << endl;
        account.showBalance();
    }
};

// ===== Main Program =====
int main() {
    Customer c1("Alice", "C001", "ACC1001", 5000);
    Customer c2("Bob", "C002", "ACC1002", 3000);

    c1.displayInfo();
    c1.account.deposit(1500);
    c1.account.withdraw(2000);
    c1.account.transfer(c2.account, 1000);

    c2.displayInfo();

    c1.account.showTransactions();
    c2.account.showTransactions();

    return 0;
}

