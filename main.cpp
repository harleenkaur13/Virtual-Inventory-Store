#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

// Base class Product
class Product {
protected:
    string name;
    double price;
    int stock;
    string expiryDate;

public:
    Product(string n, double p, int s, string e) : name(n), price(p), stock(s), expiryDate(e) {}

    virtual void display() const {
        cout << "Product: " << name << ", Price: " << price << ", Stock: " << stock << ", Expiry Date: " << expiryDate << endl;
    }

    string getName() const { return name; }
    int getStock() const { return stock; }
    double getPrice() const { return price; }
    string getExpiryDate() const { return expiryDate; }
    void reduceStock(int quantity) { stock -= quantity; }
};

// Derived class for Grocery products
class Grocery : public Product {
public:
    Grocery(string n, double p, int s, string e) : Product(n, p, s, e) {}

    void display() const override {
        cout << "Grocery Product: " << name << ", Price: " << price << ", Stock: " << stock << ", Expiry Date: " << expiryDate << endl;
    }
};

// Derived class for Electronics products
class Electronics : public Product {
public:
    Electronics(string n, double p, int s, string e) : Product(n, p, s, e) {}

    void display() const override {
        cout << "Electronics Product: " << name << ", Price: " << price << ", Stock: " << stock << ", Expiry Date: " << expiryDate << endl;
    }
};

// Derived class for Furniture products
class Furniture : public Product {
public:
    Furniture(string n, double p, int s, string e) : Product(n, p, s, e) {}

    void display() const override {
        cout << "Furniture Product: " << name << ", Price: " << price << ", Stock: " << stock << ", Expiry Date: " << expiryDate << endl;
    }
};

// Category class to hold different products
class Category {
private:
    string name;
    vector<Product*> products;

public:
    Category(string n) : name(n) {}

    void addProduct(Product* product) {
        products.push_back(product);
    }

    void display() const {
        cout << "--- " << name << " ---" << endl;
        cout << left << setw(20) << "Product Name"
             << setw(10) << "Price"
             << setw(10) << "Stock"
             << setw(15) << "Expiry Date" << endl;
        cout << string(65, '-') << endl;

        for (const auto& product : products) {
            cout << left << setw(20) << product->getName()
                 << setw(10) << product->getPrice()
                 << setw(10) << product->getStock()
                 << setw(15) << product->getExpiryDate() << endl;
        }
    }

    string getName() const { return name; }

    Product* getProduct(const string& productName) {
        for (auto& product : products) {
            string prodName = product->getName();
            string inputName = productName;

            transform(prodName.begin(), prodName.end(), prodName.begin(), ::tolower);
            transform(inputName.begin(), inputName.end(), inputName.begin(), ::tolower);

            if (prodName == inputName) {
                return product;
            }
        }
        return nullptr;
    }

    vector<Product*>& getProducts() {
        return products;
    }
};

// Store class to manage categories and sales
class Store {
private:
    vector<Category*> categories;
    vector<string> transactions;

public:
    ~Store() {
        for (auto& category : categories) {
            delete category;
        }
    }

    void addCategory(Category* category) {
        categories.push_back(category);
    }

    void displayInventory() const {
        for (const auto& category : categories) {
            category->display();
        }
    }

    void makeSale(const string& category, const string& productName, int quantity) {
        for (auto& cat : categories) {
            if (cat->getName() == category) {
                Product* product = cat->getProduct(productName);
                if (product && product->getStock() >= quantity) {
                    product->reduceStock(quantity);
                    double totalPrice = product->getPrice() * quantity;
                    cout << "Sale made: " << productName << " x" << quantity << " for total price: " << totalPrice << endl;
                    transactions.push_back("Sold: " + productName + " x" + to_string(quantity) + " for " + to_string(totalPrice));
                    return;
                } else if (!product) {
                    cout << "Product not found." << endl;
                } else {
                    cout << "Not enough stock available." << endl;
                }
            }
        }
        cout << "Category not found." << endl;
    }

    void generateReports() const {
        cout << "\n--- Transaction Report ---" << endl;
        cout << left << setw(20) << "Transaction Type"
             << setw(20) << "Product Name"
             << setw(10) << "Quantity"
             << setw(15) << "Total Price" << endl;
        cout << string(65, '-') << endl;

        for (const auto& transaction : transactions) {
            string transactionType, productName;
            int quantity = 0;
            double totalPrice = 0.0;

            // Parse the transaction string
            size_t pos1 = transaction.find(": ");  
            size_t pos2 = transaction.find(" x", pos1);
            size_t pos3 = transaction.find(" for ", pos2); 

            if (pos1 != string::npos && pos2 != string::npos && pos3 != string::npos) {
                transactionType = transaction.substr(0, pos1 + 1);  
                productName = transaction.substr(pos1 + 2, pos2 - pos1 - 2);  
                quantity = stoi(transaction.substr(pos2 + 2, pos3 - pos2 - 2));  
                totalPrice = stod(transaction.substr(pos3 + 5));  
            }

            // Display the transaction
            cout << left << setw(20) << transactionType
                 << setw(20) << productName
                 << setw(10) << quantity
                 << setw(15) << fixed << setprecision(2) << totalPrice << endl;
        }
    }

    void saveData(const string& filename) const {
        ofstream outFile(filename);
        if (outFile) {
            for (const auto& category : categories) {
                outFile << category->getName() << endl;
                for (const auto& product : category->getProducts()) {
                    outFile << product->getName() << " " << product->getPrice() << " " << product->getStock() << " " << product->getExpiryDate() << endl;
                }
            }
            cout << "Data successfully saved to " << filename << endl;
        } else {
            cout << "Error saving data." << endl;
        }

        ofstream transFile("transactions.txt");
        for (const auto& transaction : transactions) {
            transFile << transaction << endl;
        }
    }

    void loadData(const string& filename) {
        ifstream inFile(filename);
        if (inFile) {
            string line;
            Category* currentCategory = nullptr;
            while (getline(inFile, line)) {
                if (line.empty()) continue;
                if (line == "Electronics" || line == "Groceries" || line == "Furniture") {
                    currentCategory = new Category(line);
                    addCategory(currentCategory);
                } else {
                    string name;
                    double price;
                    int stock;
                    string expiryDate;
                    stringstream ss(line);
                    ss >> name >> price >> stock >> expiryDate;
                    Product* product = nullptr;

                    if (currentCategory->getName() == "Electronics") {
                        product = new Electronics(name, price, stock, expiryDate);
                    } else if (currentCategory->getName() == "Groceries") {
                        product = new Grocery(name, price, stock, expiryDate);
                    } else if (currentCategory->getName() == "Furniture") {
                        product = new Furniture(name, price, stock, expiryDate);
                    }

                    if (product) {
                        currentCategory->addProduct(product);
                    }
                }
            }
            cout << "Data successfully loaded from " << filename << endl;
        } else {
            cout << "Error loading data." << endl;
        }
    }
};

int main() {
    Store store;

    store.loadData("store_data.txt");

    int choice;
    do {
        cout << "\n--- Virtual Store Inventory System ---\n";
        cout << "1. Display Inventory\n";
        cout << "2. Make a Sale\n";
        cout << "3. Generate Transaction Report\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            store.displayInventory();
            break;
        case 2: {
            string category, productName;
            int quantity;
            cout << "Enter category: ";
            cin >> category;
            cout << "Enter product name: ";
            cin >> productName;
            cout << "Enter quantity: ";
            cin >> quantity;
            store.makeSale(category, productName, quantity);
            break;
        }
        case 3:
            store.generateReports();
            break;
        case 4:
            cout << "Exiting the system. Goodbye!" << endl;
            break;
        default:
            cout << "Invalid choice. Please try again." << endl;
            break;
        }
    } while (choice != 4);

    store.saveData("store_data.txt");

    return 0;
}
