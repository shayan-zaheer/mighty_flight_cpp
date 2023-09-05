#include<iostream>
#include<conio.h>
#include<fstream>
#include<curl/curl.h>
#include<json/json.h>
#include<jsoncpp.cpp>
using namespace std;

string apiKey = "4ddce66d-a75d-400c-b4d5-433891bdf0cf";
string url_france[5] = {"https://airlabs.co/api/v9/flight?flight_iata=AF10&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AF1061&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AF1515&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AF1341&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AF11&api_key=" + apiKey};
string url_spain[5] = {"https://airlabs.co/api/v9/flight?flight_iata=VY5510&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=VY5167&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=VY3915&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=VY1868&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=VY6101&api_key=" + apiKey};
string url_china[5] = {"https://airlabs.co/api/v9/flight?flight_iata=CA166&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=CA841&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=CA8323&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=CA126&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=CA120&api_key=" + apiKey};
string url_italy[5] = {"https://airlabs.co/api/v9/flight?flight_iata=AZ109&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AZ1430&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AZ207&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AZ1710&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=AZ641&api_key=" + apiKey};
string url_usa[5] = {"https://airlabs.co/api/v9/flight?flight_iata=DL109&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=DL1430&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=DL207&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=DL133&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=DL1564&api_key=" + apiKey};
string url_thailand[5] = {"https://airlabs.co/api/v9/flight?flight_iata=TG417&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=TG322&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=TG207&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=TG324&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=TG660&api_key=" + apiKey};
string url_germany[5] = {"https://airlabs.co/api/v9/flight?flight_iata=LH109&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=LH1430&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=LH207&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=LH103&api_key=" + apiKey, "https://airlabs.co/api/v9/flight?flight_iata=LH1703&api_key=" + apiKey};

void menu();
void goBack();

class User{
protected:
    int age;
    static string name;
    string address, gender;
public:
    void info(){
        cout << "Enter customer's name:\n";
        cin.ignore();
        getline(cin, name);
        ifstream in(name + "_info.txt");
        if(in.good()){
            cout << "You have already entered your info.";
            goBack();
        }
        cout << "Enter customer's age:\n";
        cin >> age;
        cout << "Enter customer's address:\n";
        cin.ignore();
        getline(cin, address);
        cout << "Enter customer's gender:\n";
        cin >> gender;
        cout << "Your information is saved!" << "\n";
        
        ofstream out(name + "_info.txt");
		out << "Name: " << name << "\n";
		out << "Age: " << age << "\n";
		out << "Address: " << address << "\n";
		out << "Gender: " << gender << "\n";  
        out.close();
    }
};

string User::name;

class AirLabsAPI : public User{ 
public:
    static string flag;
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response) { 
        size_t totalSize = size * nmemb;
        response->append((char*)contents, totalSize);
        return totalSize;
    }

    void getFlightInfo(string& url) {
        CURL* curl = curl_easy_init();
        if (curl) {
            string response;
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);

            CURLcode res = curl_easy_perform(curl);

            if(res == CURLE_OK){
                if(!response.empty()){
                    Json::CharReaderBuilder builder;
                    Json::Value root;
                    string errors;

                    unique_ptr<Json::CharReader> reader(builder.newCharReader());

                    if (reader->parse(response.c_str(), response.c_str() + response.size(), &root, &errors)) {
                        if (root.isMember("error")){
                            string errorMessage = root["error"]["message"].asString();
                            cerr << "\nFlight not found. Error: " << errorMessage << "\n";
                        }
                        else{
                            Json::Value flightInfo = root["response"];

                            string flightName = flightInfo["flight_icao"].asString();
                            string arrivalCity = flightInfo["arr_city"].asString();
                            string departureCity = flightInfo["dep_city"].asString();
                            string arrivalAirport = flightInfo["arr_name"].asString();
                            string departureAirport = flightInfo["dep_name"].asString();
                            flag = flightInfo["flag"].asString();

                            ofstream out("ticket" + flag + "_info.txt", ios::app);
                            static int flight_count = 1;
                            cout << "\n";

                            cout << flight_count << ". ";
                            out << flight_count << ". ";

                            flight_count++;
                            cout << "Flight Name: " << flightName << "\n";
                            out << "Flight Name: " << flightName << "\n";
                            cout << "Arrival City: " << arrivalCity << "\n";
                            out << "Arrival City: " << arrivalCity << "\n";
                            cout << "Departure City: " << departureCity << "\n";
                            out << "Departure City: " << departureCity << "\n";
                            cout << "Arrival Airport: " << arrivalAirport << "\n";
                            out << "Arrival Airport: " << arrivalAirport << "\n";
                            cout << "Departure Airport: " << departureAirport << "\n";
                            out << "Departure Airport: " << departureAirport << "\n\n";
                            out.close();
                        }
                    }
                    else{
                        cerr << "Failed to parse JSON response: " << errors << "\n";
                    }
                }
                else{
                    cerr << "Empty response received from the API.\n";
                }
            }
            else{
                cerr << "Request failed: " << curl_easy_strerror(res) << "\n";
            }
            curl_easy_cleanup(curl);
        }
    }
};

string AirLabsAPI::flag;

class Ticket : public AirLabsAPI{
protected:
    int countryChoice;
    string ticketChoice;
    bool found;
public:
    void reserve(){
        string line;
        string* country = new string[8]{"France", "Spain", "China", "Italy", "USA", "Thailand", "Germany"}; 
        AirLabsAPI api;
        cout << "Ticket Reservation:\n";
        for(int i=0; i<7; i++) {
            cout << (i + 1) << ". Flight to " << country[i] << "\n";
        }
        delete[] country;

        cout << "Enter the number of country you want to visit:\n";
        cin >> countryChoice;

        switch(countryChoice){
        case 1:
        {
            cout << "Welcome to Air France\n";
            cout << "France is in the air\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++)
            {
                api.getFlightInfo(url_france[i]); 
            }
            
            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);
            out << "The selected flight:\n";

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
        }
        case 2:
    	{
            cout << "Welcome to Vueling Airlines\n";
            cout << "We Love Places\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++)
            {
                api.getFlightInfo(url_spain[i]);
            }

            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);
            out << "The selected flight:\n";

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
		}
        case 3:
        {
            cout << "Welcome to Air China\n";
            cout << "Journey with a caring smile\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++){
                api.getFlightInfo(url_china[i]);
            }

            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);
            out << "The selected flight:\n";

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
        }
        case 4:
        {
            cout << "Welcome to ITA Airways\n";
            cout << "Born to be sustainable\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++){
                api.getFlightInfo(url_italy[i]);
            }

            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);
            out << "The selected flight:\n";

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
        }
        case 5:
        {
            cout << "Welcome to Delta Airlines\n";
            cout << "Airline of the South\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++){
                api.getFlightInfo(url_usa[i]);
            }

            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);
            out << "The selected flight:\n";

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
        }
        case 6:
        {
            cout << "Welcome to Thai Airways\n";
            cout << "Smooth as silk\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++){
                api.getFlightInfo(url_thailand[i]);
            }

            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);
            out << "The selected flight:\n";

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
        }
        case 7:
        {
            cout << "Welcome to Deutsche Lufthansa\n";
            cout << "There's no better way to fly\n";
            cout << "Following are the flights:\n";
            for(int i=0; i<5; i++){
                api.getFlightInfo(url_germany[i]);
            }

            cout << "\nEnter the ticket number:\n";
            cin >> ticketChoice;

            cout << "You have selected:\n";
            ifstream in("ticket" + api.flag + "_info.txt");
            ofstream out(name + "_info.txt", ios::app);

            cout << "\n";
            while(getline(in, line)){
                if(line.find(ticketChoice + ". ") != string::npos)
                {
                    found = true;
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && (!line.empty()))
                {
                    cout << line << "\n";
                    out << line << "\n";
                }
                else if(found && line.empty())
                {
                    cout << "\n";
                    break;
                }
            }
            out.close();
            in.close();
            break;
        }
        default:
        {
            cout << "Invalid input! Try again\n";
            reserve();
        }
    }
}
};

class Charge : public User{
private:
    char choice;
public:
    void bill(){
        ofstream out(name + "_info.txt", ios::app);
        srand(time(0));
        int charge = rand()%(1000-300+1)+300;
        cout << "The charges for the selected flight: $" << charge << "\n";
        out << "Charges: " << charge << "$";
        cout << "\nYour flight has been booked!\n";
        cout << "Press any key to see your receipt!\n";
        cin.ignore();
        cin.get();
    }

    void read(){
        string line;
        ifstream in(name + "_info.txt");
        cout << "\n=== RECEIPT ===\n";
        while(getline(in, line)){
             cout << line << "\n";
        }
        in.close();
    }
};

void goBack(){
    cout << "Press any key to go to main menu:\n";
    getch();
    menu();
}

// initializing the instances of my classes
User detail;
Ticket ticket;
Charge charge;

void menu(){
    int choice;
    cout << "\nPress 1 to enter Customer details.\n";
    cout << "Press 2 to reserve tickets.\n";
    cout << "Press 3 for the Ticket charges.\n";
    cout << "Press 4 to exit the program.\n";
    cin >> choice;

    switch(choice){
    case 1:
    {
        detail.info();
        goBack();
        break;
    }
    case 2:
    {
        ticket.reserve();
        goBack();
        break;
    }
    case 3:
    {
        charge.bill();
        charge.read();
        goBack();
        break;
    }
    case 4:
    {
        cout << "\nExiting the program....\n";
        exit(1);
    }
    }
}

int main()
{
    menu();
    return 0;
}