#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <iterator>

using namespace std;
typedef vector<string> arraystring;
vector<arraystring> data;

class Customer{
public:

    int id;
    float arrivelTime;
    float orderTime;
    float brewTime;
    float price;
    float baristaTime;
    float cashierTime;
    float timeWhenCofeeDone;
    int cashierNumber;


    Customer();
    Customer(int id,float arrivelTime, float orderTime, float brewTime, float price);
    ~Customer();


    void setTimeWhenCofeeDone();
    void setCashierTime();

};

Customer::Customer(){

    this->arrivelTime= 0;
    this->orderTime=0;
    this->brewTime = 0;
    this->price = 0;
    this->id=0;

}

Customer::Customer(int id,float arrivelTime, float orderTime, float brewTime, float price){
    this->arrivelTime= arrivelTime;
    this->orderTime=orderTime;
    this->brewTime = brewTime;
    this->price = price;
    this->id = id;
}

Customer::~Customer(){};


void Customer::setTimeWhenCofeeDone(){
    this->timeWhenCofeeDone =  this->baristaTime - this->arrivelTime;
};

void Customer::setCashierTime(){
    this->cashierTime = this->arrivelTime+ this->orderTime;
}



class Cashier
{
public:
    int id;
    float busyTime;
    float cashierTime;

    Cashier()
    {
        id = 0;
        busyTime = 0;
        cashierTime =0;

    }

    Cashier(int id, float busyTime, float cashierTime)
    {
        this->id = id;
        this->busyTime = busyTime;
        this->cashierTime=cashierTime;
    }

    ~Cashier()
    {
    }

    int getId()
    {
        return this->id;
    }

};




class Barista
{
public:
    int id;
    float busyTime;
    float baristaTime;

    Barista()
    {
        id = 0;
        busyTime = 0;
        baristaTime=0;

    }

    Barista(int id, float busyTime, float baristaTime)
    {
        this->id = id;
        this->busyTime = busyTime;
        this->baristaTime= baristaTime;
    }

    ~Barista()
    {
    }

    int getId()
    {
        return this->id;
    }

};


struct Node{

    Customer customer;
    struct Node* next;
};



class Queue{
public:

    struct Node* front = 0;
    struct Node* rear = 0;
    int size = 0;

    Queue(){
        front= 0;
        rear = 0;
        size = 0;
    }
    ~Queue(){}

    void enqueue(Customer customer){
        struct Node* temp = new struct Node;
        temp->customer = customer;
        temp->next = 0;
        if ( front ==0 && rear == 0){
            this->size += 1;
            front=rear=temp;
            return;
        }
        rear->next= temp;
        rear = temp;
        this->size+=1;
    }

    void dequeue(){
        struct Node* temp = front;
        if(front == 0){
            return;
        }
        if(front == rear){
            front = rear = 0;
            this->size += -1;
        }
        else{
            front= front->next;
        }
        delete temp;
        this->size+= -1;
    }

};


template <typename T>  // for remove first two customers from customers vector
void remove(std::vector<T>& v, size_t index) {
    v.erase(v.begin() + index);
}



double round(double var)
{

    double value = (int)(var * 100 + .5);
    value = value / 100;
    return value;
	
	    
}


int main(int argc, char *argv[]) {

    Queue cashierQueue = *new Queue;
    Queue baristaQueue = *new Queue;


    int numberOfLine = 0;       // for finding number of customers


    fstream myfile;
    myfile.open(argv[1], ios::in);        //open an input file
    ofstream outputFile(argv[2]);               //open ana output file
    if(!myfile){
        cout<< "no such file!";
    }
    else {                                          //inserting the input data to 2d matrix
        string line, temp;
        while (getline(myfile, line)) {
            numberOfLine++;

            data.push_back(arraystring ());


            istringstream strm(line);
            while (strm >> temp)

                data.back().push_back(temp);

        }
    }

    int numberOfCashier = stoi(data[0][0]);
    vector<Cashier> cashiers ;
    for(int i = 1 ; i <= numberOfCashier; i++ ){

        Cashier cashier = *new Cashier;
        cashier.id = i;
        cashiers.push_back(cashier);
    }





    int numberOfBarista = numberOfCashier / 3;
    vector<Barista> baristas;
    for(int i = 1 ; i <= numberOfBarista ; i++){
        Barista barista = *new Barista;
        barista.id = i;
        baristas.push_back(barista);
    }


    vector<Customer> customers;
    for ( int i = 1; i <= numberOfLine-2; i++){
        Customer customer(i, stof(data[i+1][0]) , stof(data[i+1][1]),stof(data[i+1][2]),stof(data[i+1][3]));
        customers.push_back(customer);

    }


    for(int i = 0; i < numberOfCashier;i++){     // adding first customers to the cashiers
        customers[i].setCashierTime();
        cashiers[i].busyTime+= customers[i].orderTime;
        cashiers[i].cashierTime += customers[i].cashierTime;
    }


    int a = numberOfCashier;
                                                                              // loop for adding a customer to cashier queue
    for(int i = 0; i< (numberOfLine -2) - numberOfCashier;i++){ // 10 - 6

        if((numberOfLine-2)>numberOfCashier){

            sort(cashiers.begin(), cashiers.end(), [](const Cashier& lhs, const Cashier& rhs) {
                return lhs.cashierTime < rhs.cashierTime;
            });



            for(int j = 0; j < numberOfCashier; j++ ){

                if ( customers[a].arrivelTime < cashiers[j].cashierTime){

                    cashierQueue.enqueue(customers[a]);
                    cashiers[j].busyTime += customers[a].orderTime;
                    cashiers[j].cashierTime += customers[a].orderTime;
                    customers[a].cashierTime = cashiers[j].cashierTime;


                }
                break;
            }
            a++;
        }
    }





    int b = numberOfCashier;           // loop for adding  customers in queue to barista
    for(int i = 0; i< (numberOfLine -2) - numberOfCashier;i++){

        if((numberOfLine-2)>numberOfCashier){

            sort(cashiers.begin(), cashiers.end(), [](const Cashier& lhs, const Cashier& rhs) {
                return lhs.id < rhs.id;
            });


            for(int j = 0; j < numberOfCashier; j++ ){

                if ( customers[b].arrivelTime >= cashiers[j].cashierTime){

                    float spaceTime = customers[b].arrivelTime - cashiers[j].cashierTime;
                    cashiers[j].busyTime += customers[b].orderTime;
                    cashiers[j].cashierTime += customers[b].orderTime + spaceTime;
                    customers[b].cashierTime = cashiers[j].cashierTime;
                    break;
                }
            }
            b++;

        }
    }


   sort(customers.begin(), customers.end(), [](const Customer& lhs, const Customer& rhs) { // sorting customers by their time to quit the cashier
       return lhs.cashierTime < rhs.cashierTime;
    });


    for(int j = 0; j < baristas.size() ; j++){   // loop for adding  first two customer to barista

        baristas[j].busyTime += customers[j].brewTime;
        baristas[j].baristaTime+= customers[j].cashierTime + customers[j].brewTime;
        customers[j].baristaTime += baristas[j].baristaTime;
        customers[j].setTimeWhenCofeeDone();

    }

    vector<Customer> firstTwoCustomer;
    firstTwoCustomer.push_back(customers[0]);
    firstTwoCustomer.push_back(customers[1]);
    remove(customers,0);
    remove(customers,0);





   vector<Customer> vectorQueueBarista;     // for sorting customers by their price

   for(int i = 0 ; i < customers.size(); i++){          //loop for adding a customer to barista queue

       sort(baristas.begin(), baristas.end(), [](const Barista& lhs, const Barista& rhs) {  // sorting customers by their time to quit the cashier
           return lhs.baristaTime < rhs.baristaTime;
       });
       for(int j = 0; j < baristas.size() ; j++){


           if(customers[i].cashierTime < baristas[j].baristaTime){


               vectorQueueBarista.push_back(customers[i]);

               sort(vectorQueueBarista.begin(), vectorQueueBarista.end(), [](const Customer& lhs, const Customer& rhs) { // sorting customers by their time to quit the cashier
                   return lhs.price > rhs.price;
               });

               baristaQueue.enqueue(customers[i]);



               for(int k = 0; k < baristas.size(); k++){
                   if(customers[i+1].cashierTime >= baristas[k].baristaTime ){
                       for (int l = 0; l < customers.size(); ++l) {

                           if(vectorQueueBarista[0].id == customers[l].id){

                                baristas[k].busyTime += customers[l].brewTime;
                                baristas[k].baristaTime += customers[l].brewTime;
                                customers[l].baristaTime=  baristas[k].baristaTime;

                                customers[l].setTimeWhenCofeeDone();
                                remove(vectorQueueBarista,0);
                                baristaQueue.dequeue();
                                break;
                           }

                       }

                   }
               }
               break;
           }
       }
   }



    sort(vectorQueueBarista.begin(), vectorQueueBarista.end(), [](const Customer& lhs, const Customer& rhs) { // sorting customers by their time to quit the cashier
        return lhs.price > rhs.price;
    });


    int vectorQueueBaristaSize = vectorQueueBarista.size();

    for (int i = 0; i < vectorQueueBaristaSize; ++i) {

        sort(baristas.begin(), baristas.end(), [](const Barista& lhs, const Barista& rhs) {  // sorting customers by their time to quit the cashier
            return lhs.baristaTime < rhs.baristaTime;
        });


            for (int k = 0; k < customers.size(); ++k) {

                if(vectorQueueBarista[0].id== customers[k].id){

                    baristas[0].busyTime += customers[k].brewTime;
                    baristas[0].baristaTime += customers[k].brewTime;
                    customers[k].baristaTime  =  baristas[0].baristaTime;

                    customers[k].setTimeWhenCofeeDone();
                    remove(vectorQueueBarista,0);
                    break;
                }
            }
    }


    for (int i = 0; i < firstTwoCustomer.size(); ++i) {

        customers.push_back(firstTwoCustomer[i]);

    }



    sort(customers.begin(), customers.end(), [](const Customer& lhs, const Customer& rhs) {  // sorting customers by their time to quit the cashier
        return lhs.id < rhs.id;
    });

    sort(baristas.begin(), baristas.end(), [](const Barista& lhs, const Barista& rhs) {  // sorting customers by their time to quit the cashier
        return lhs.baristaTime > rhs.baristaTime;
    });

    float totalRuningTime = baristas[0].baristaTime;

    outputFile << baristas[0].baristaTime<<endl;
    outputFile <<cashierQueue.size<<endl;
    outputFile <<baristaQueue.size<<endl;




    for(auto it = cashiers.begin(); it !=cashiers.end(); it++) {
        double x = it -> busyTime / totalRuningTime;
        outputFile << round(x)<< endl;

  }

    for(auto it = baristas.begin(); it !=baristas.end(); it++) {
        double y = it -> busyTime / totalRuningTime;
        outputFile << round( y )<< endl;

    }

    for(auto it = customers.begin(); it !=customers.end(); it++) {
        outputFile <<  it -> baristaTime - it->arrivelTime << endl;

    }

    outputFile <<endl;






    //************************ model2 *************************



    vector<Cashier> cashiers2 ;
    for(int i = 1 ; i <= numberOfCashier; i++ ){

        Cashier cashier = *new Cashier;
        cashier.id = i;
        cashiers2.push_back(cashier);
    }




    vector<Barista> baristas2;
    for(int i = 1 ; i <= numberOfBarista ; i++){
        Barista barista = *new Barista;
        barista.id = i;
        baristas2.push_back(barista);
    }


    vector<Customer> customers2;
    for ( int i = 1; i <= numberOfLine-2; i++){
        Customer customer(i, stof(data[i+1][0]) , stof(data[i+1][1]),stof(data[i+1][2]),stof(data[i+1][3]));
        customers2.push_back(customer);

    }


    for(int i = 0; i < numberOfCashier;i++){
        customers2[i].setCashierTime();
        cashiers2[i].busyTime+= customers2[i].orderTime;
        cashiers2[i].cashierTime += customers2[i].cashierTime;
        customers2[i].cashierNumber = i+1;
    }



    Queue cashierQueue2 = *new Queue;
    int a2 = numberOfCashier;

    // loop for adding a customer to cashier queue
    for(int i = 0; i< (numberOfLine -2) - numberOfCashier;i++){ // 10 - 6

        if((numberOfLine-2)>numberOfCashier){

            sort(cashiers2.begin(), cashiers2.end(), [](const Cashier& lhs, const Cashier& rhs) {
                return lhs.cashierTime < rhs.cashierTime;
            });



            for(int j = 0; j < numberOfCashier; j++ ){

                if ( customers2[a2].arrivelTime < cashiers2[j].cashierTime){

                    cashierQueue2.enqueue(customers2[a2]);
                    cashiers2[j].busyTime += customers2[a2].orderTime;
                    cashiers2[j].cashierTime += customers2[a2].orderTime;
                    customers2[a2].cashierTime = cashiers2[j].cashierTime;
                    customers2[a2].cashierNumber = j+1;


                }
                break;
            }
            a2++;
        }
    }





    int b2 = numberOfCashier; // loop for other line of customer
    for(int i = 0; i< (numberOfLine -2) - numberOfCashier;i++){

        if((numberOfLine-2)>numberOfCashier){

            sort(cashiers2.begin(), cashiers2.end(), [](const Cashier& lhs, const Cashier& rhs) {
                return lhs.id < rhs.id;
            });


            for(int j = 0; j < numberOfCashier; j++ ){

                if ( customers2[b2].arrivelTime >= cashiers2[j].cashierTime){

                    float spaceTime = customers[b2].arrivelTime - cashiers2[j].cashierTime;
                    cashiers2[j].busyTime += customers2[b2].orderTime;
                    cashiers2[j].cashierTime += customers2[b2].orderTime + spaceTime;
                    customers2[b2].cashierTime = cashiers2[j].cashierTime;
                    customers2[b2].cashierNumber= j+1;
                    break;
                }
            }
            b2++;

        }
    }


    for (int i = 1; i <= numberOfCashier; i = i+3) {  //  barista's first customer

        sort(baristas2.begin(), baristas2.end(), [](const Barista& lhs, const Barista& rhs) {
            return lhs.baristaTime < rhs.baristaTime;
        });

        for (int j = 0; j < customers2.size(); ++j) {

            if( i == customers2[j].id){

                for (int k = 0; k < baristas2.size(); ++k) {

                    baristas2[k].baristaTime += customers2[j].cashierTime;

                    break;
                }
            }
        }
    }



    sort(customers2.begin(), customers2.end(), [](const Customer& lhs, const Customer& rhs) {
        return lhs.cashierNumber < rhs.cashierNumber;
    });




    int c = -2;  // cashierNumber of customer
    int m = 1;  // for loop
    int n = -1; //  vector of baristas2 index
    for (int i = 0; i < numberOfCashier ; i =i+3) {

        n++;
        m+=3;
        c += 3;
        while (c < m){

            for (int j = 0; j < customers2.size(); ++j) {

                if(c== customers2[j].cashierNumber){

                    baristas2[n].baristaTime += customers2[j].brewTime;
                    baristas2[n].busyTime += customers2[j].brewTime;

                }

            }
            c++;
        }

    }






    outputFile << -1 << endl;
    outputFile<< cashierQueue2.size<<endl;




    outputFile.close();
    myfile.close();
    return 0;
}
