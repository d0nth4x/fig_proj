#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

void btn(){
    char x;
    cout << "\n Wpisz dowolny znak, żeby kontynuować \n";
    cin >> x;
}

class Klienci{
    
public:
    int id;
    string imie,nazwisko;
    
    Klienci(){
        //        imie        = new string("brak");
        //        nazwisko    = new string("brak");
        //        id          = new int(0);
        imie        = "brak";
        nazwisko    = "brak";
        id          = 0;
    }
    
    void zmienImie(string _imie){
        this->imie = _imie;
    }
    void zmienNazwisko(string _nazwisko){
        nazwisko = _nazwisko;
    }
    int podajId(){return id;}
    
    void virtual wypiszDane() = 0;
    void virtual zmienFirme(string n) = 0;
    void virtual serialize(ostream& stream) = 0;
    //void virtual deSerialize(istream& stream) = 0;
    
    virtual ~Klienci(){
        //        delete id;
        //        delete imie;
        //        delete nazwisko;
    }
    
    friend bool operator == ( const Klienci &n1, const Klienci &n2);
};

bool operator == ( const Klienci &n1, const Klienci &n2){
    return n1.id == n2.id;
}

class Indywid:public Klienci{
public:
    Indywid(){
        imie        = "brak";
        nazwisko    = "brak";
        id          = 0;
    };
    Indywid(int _id){
        id = _id;
    }
    Indywid(int _id,string _imie,string _nazwisko){
        this->id = _id;
        this->imie = _imie;
        this->nazwisko = _nazwisko;
    }
    
    void serialize(ostream& stream){
        stream << id << "," << imie << "," << nazwisko << endl;
    }
    ~Indywid(){
        cout << "usuwam nic\n";
        
    }
    
    void wypiszDane(){
        cout << id << "\t " << nazwisko << "\t" << imie << endl;
    }
    void zmienFirme(string n){
        cerr << "Blad: to nie klient biznesowy!\n";
    };
    
};

class Firma:public Klienci{
    string firma;
public:
    Firma(){
        this->id = 0;
        this->imie = "";
        this->nazwisko = "";
        this->firma = "";
    }
    Firma(int _id, string _imie, string _nazwisko, string _firma){
        this->id = _id;
        this->imie = _imie;
        this->nazwisko = _nazwisko;
        this->firma = _firma;
    }
    void serialize(ostream& stream){
        stream << id << "," << imie << "," << nazwisko << "," << firma << endl;
    }
    
    ~Firma(){
        //delete firma;
    }
    
    void wypiszDane(){
        cout << id << "\t " << nazwisko << "\t " << imie << "\t " << firma << endl;
    }
    
    void zmienFirme(string _firma){
        firma = _firma;
    }
};

class Urzadzenie{
public:
    int id;
    string nazwa;
    Urzadzenie(){
        id = 0;
        nazwa = "";
    }
    
    Urzadzenie( int _id, string _nazwa){
        this->id = _id;
        this->nazwa = _nazwa;
    };
    
    void wyswietl(){
        cout << "xx" << endl;
    };
    void zmienNazwe(string _nazwa){
        this->nazwa = _nazwa;
    }
    
    friend bool operator== ( const Urzadzenie &n1, const Urzadzenie &n2);
    
    void zmienKlienta();
    void edytuj();
    
    //virtual ~Urzadzenie();
    
};

class Biurowe : public Urzadzenie{
    Urzadzenie *u;
public:
    void edytuj(int _id, string _nazwa){
        id = _id;
        nazwa = _nazwa;
    };
    
    void wyswietl(){
        cout << u->id << "\t " << u->nazwa << endl;
    };
    Biurowe() :Urzadzenie(){}
    
    
    Biurowe(int id, string nazwa) :Urzadzenie(id, nazwa){}
};


class Naprawa : public Urzadzenie{ // urzadzenie klienta
private:
    Urzadzenie *u;
    int klientId;
public:
    Naprawa() : Urzadzenie(){
        this->klientId = 0;
    }
    Naprawa(int _id, string _nazwa, int _klientId) :Urzadzenie( _id, _nazwa){
        this->klientId = _klientId;
    };
    
    void edytuj(int _id, string _nazwa, int _klientId){
        id = _id;
        nazwa = _nazwa;
        klientId = _klientId;
    };
    
    void zmienKlienta(int _id){
        this->klientId = _id;
    }
    void wyswietl(){
        cout << id << "\t " << nazwa << "\t " << klientId << endl;
    }
    
    int podajKlientId(){
        return klientId;
    }
};
vector<Klienci*>    klienciTab;
vector<Klienci*>       klienciFirmaTab;
vector<Urzadzenie>  urzadzeniaTab;
vector<Naprawa>     urzadzeniaKlienciTab;


void save(){
    fstream file;
    file.open("klienci.csv", ios::out);
    if(!file)
        cerr << "Nie mogę edytować pliku klienci.csv";
    
    if(klienciTab.size()>0)
        for(int i = 0; i<= klienciTab.size()-1; i++){
            Klienci *_k = new Indywid();
            _k = klienciTab[i];
            _k->serialize(file);
        }
    
    file.clear();
    file.close();
    
    file.open("klienci_firma.csv", ios::out);
    if(!file)
        cerr << "Nie mogę edytować pliku klienci_firma.csv";
    
    if(klienciTab.size()>0)
        for(int i = 0; i<= klienciFirmaTab.size()-1; i++){
            Klienci *_kf = new Firma();
            _kf = klienciFirmaTab[i];
            
            _kf->serialize(file);
        }
    
    file.clear();
    file.close();
     
     file.open("dev.bin", ios::out | ios::binary | ios::trunc);
     if(file.good()){
     if(urzadzeniaTab.size() > 0){
     for(int i = 0; i<= urzadzeniaTab.size()-1; i++){
     
     file.write( (const char*)&urzadzeniaTab[i], sizeof(Naprawa) );
     }
     }
     }else
     cerr << "Nie mogę edytować pliku dev.bin";
     
     file.clear();
     file.close();
    
     file.open("dev_naprawa.bin", ios::out | ios::binary | ios::trunc);
     if(file.good()){
     if(urzadzeniaKlienciTab.size() > 0){
     for(int i = 0; i<= urzadzeniaKlienciTab.size()-1; i++){
     
     file.write( (const char*)&urzadzeniaKlienciTab[i], sizeof(Naprawa) );
     }
     }
     }else
     cerr << "Nie mogę edytować pliku dev_naprawa.bin";
     
     file.clear();
     file.close();
    
}

int main() {
    int menu,
    id = 0,     // id klienta
    devid = 0,   // id urzadzenia
    klient;     // wrapper
    string imie,nazwisko,firma,nazwa;
    size_t size;
    
    
    
    /*
     *  Ladownie danych z pliku
     */
    ifstream file;
    string s,_field[5];
    
    int count=0;
    file.open("klienci.csv", ios::out);
    if(file.good()){
        while(getline(file,s)){
            istringstream ss(s); // sprawdza czy jest jeszcze w tej samej linii
            string field;
            count = 0;
            while(getline(ss,field,',')){
                _field[++count] = field;
            }
            if(atoi(_field[1].c_str()) > id) id = atoi(_field[1].c_str());
            klienciTab.push_back( new Indywid( atoi(_field[1].c_str() ),
                                              _field[2],
                                              _field[3] ) );
            
        }
        
        
    }else
        cerr << "Nie mogę otworzyć pliku klienci.bin" << endl;
    
    file.clear();
    file.close();
    
    file.open("klienci_firma.csv", ios::out);
    if(file.good()){
        while(getline(file,s)){
            istringstream ss(s); // sprawdza czy jest jeszcze w tej samej linii
            string field;
            count = 0;
            while(getline(ss,field,',')){
                _field[++count] = field;
            }
            if(atoi(_field[1].c_str()) > id) id = atoi(_field[1].c_str());
            klienciFirmaTab.push_back( new Firma( atoi(_field[1].c_str() ),
                                                 _field[2],
                                                 _field[3],
                                                 _field[4]) );
            
        }
        
        
    }else
        cerr << "Nie mogę otworzyć pliku klienci.bin" << endl;
    
    file.clear();
    file.close();
    /*
     file.open("klienci_firma.bin", ios::out | ios::binary);
     if(file.good()){
     Firma *kv = (Firma *) new char[sizeof(Firma)];
     file.read( (char*)kv, sizeof(*kv) );
     while(!file.eof()){
     int tmp_id = kv->podajId();
     
     klienciFirmaTab.push_back( *kv );
     file.read( (char*)kv, sizeof(*kv) );
     
     if(tmp_id > id) id = tmp_id;
     }
     }else
     cerr << "Nie mogę otworzyć pliku klienci_firma.bin" << endl;
     
     file.clear();
     file.close();
     */
     file.open("dev.bin", ios::out | ios::binary);
     if(file.good()){
     Biurowe *kv = (Biurowe *) new char[sizeof(Biurowe)];
     file.read( (char*)kv, sizeof(*kv) );
     while(!file.eof()){
     int tmp_id = kv->id;
     
     urzadzeniaTab.push_back( *kv );
     file.read( (char*)kv, sizeof(*kv) );
     
     if(tmp_id > id) devid = tmp_id;
     }
     }else
     cerr << "Nie mogę otworzyć pliku dev.bin" << endl;
     
     file.clear();
     file.close();
     
     file.open("dev_naprawa.bin", ios::out | ios::binary);
     if(file.good()){
     Naprawa *kv = (Naprawa *) new char[sizeof(Naprawa)];
     file.read( (char*)kv, sizeof(*kv) );
     while(!file.eof()){
     int tmp_id = kv->id;
     
     urzadzeniaKlienciTab.push_back( *kv );
     file.read( (char*)kv, sizeof(*kv) );
     
     if(tmp_id > id) devid = tmp_id;
     }
     }else
     cerr << "Nie mogę otworzyć pliku dev_naprawa.bin" << endl;
    
    /***********************/
    
    while(true){
        system("clear");
        cout << "klienci tab: " << klienciTab.size() <<endl;
        cout << "klienci tab firma: " << klienciFirmaTab.size() <<endl;
        cout << "urzadzenia biurowe: " << urzadzeniaTab.size() << endl;
        cout << "urzadzenia do naprawy " << urzadzeniaKlienciTab.size() << endl;
        cout << "\nmenu:" << endl
        << "1. Dodaj klienta" << endl
        << "2. Wyswietl klientow" << endl
        << "3. Dodaj klienta firma" << endl
        << "4. Dodaj urzadzenie" << endl
        << "5. Wyswietl urzadzenia" << endl
        << "6. Wyszukaj i edytuj klienta " << endl
        << "0. ZAKOŃCZ\n\n";
        cin >> menu;
        
        switch (menu) {
            case 0:
                return 0;
                break;
                
            case 1:{
                system("clear");
                cout << "Podaj imie:";
                cin >> imie;
                cout << "\nPodaj nazwisko:";
                cin >> nazwisko;
                
                klienciTab.push_back(new Indywid(++id, imie, nazwisko));
                save();
                
                break;
            }
                
            case 2:{
                size = klienciTab.size();
                cout << "Klienci indywidualni\n" << "ilosc:" <<  size << endl;
                cout << "id \tImie \tNazwisko\n";
                if(size > 0)
                    for (int i=0; i<=size-1; i++){
                        Klienci *tmp = new Indywid();
                        tmp = klienciTab[i];
                        tmp->wypiszDane();
                        //cout << tmp.id << "\t" << tmp.imie << "\t" << tmp.nazwisko << endl;
                    }
                
                size = klienciFirmaTab.size();
                cout << "\nKlienci biznesowi\n" << "ilosc:" <<  size << endl;
                cout << "id \tImie \tNazwisko \tfirma \n";
                if(size > 0)
                    for (int i=0; i<=size-1; i++){
                        Klienci *tmp = new Firma();
                        tmp = klienciFirmaTab[i];
                        
                        tmp->wypiszDane();
                        //cout << tmp.id << "\t" << tmp.imie << "\t" << tmp.nazwisko << "\t" << tmp.wypisz_firma() << endl;
                    }
                
                btn();
                break;
            }
                
            case 3:{
                system("clear");
                cout << "Podaj imie: ";
                cin >> imie;
                cout << "\nPodaj nazwisko: ";
                cin >> nazwisko;
                cout << "\nPodaj nazwę firmy: ";
                cin >> firma;
                
                //dodajKlienta(++id, imie, nazwisko,firma);
                klienciFirmaTab.push_back( new Firma(++id,imie,nazwisko,firma) );
                save();
                break;
            }
            case 4:{
                system("clear");
                cout << "\nWpisz nazwe urzadzenia: ";
                cin >> nazwa;
                cout << "Wpisz nr. klienta. 0 - jeżeli brak";
                cin >> klient;
                
                if(klient == 0){
                    Urzadzenie *tmp = new Biurowe(++devid,nazwa);
                    urzadzeniaTab.push_back(*tmp);
                    delete tmp;
                    
                }else{
                    Naprawa *tmp = new Naprawa(++devid, nazwa, klient);
                    urzadzeniaKlienciTab.push_back(*tmp);
                    delete tmp;
                }
                
                save();
                break;
            }
                
            case 5:{
                system("clear");
                cout << "urzadzenia firmowe:\n";
                size = urzadzeniaTab.size();
                if(size > 0){
                    cout << "id\t nazwa" << endl;
                    for (int i=0; i<=size-1; i++){
                        Urzadzenie utemp;
                        utemp = urzadzeniaTab[i];
                        //Biurowe tmp = urzadzeniaTab[i];
                        cout << utemp.id << " " << utemp.nazwa << endl;
                    }
                }else
                    cout << "brak\n\n";
                
                
                cout << "\nurzadzenia do naprawy:\n";
                size = urzadzeniaKlienciTab.size();
                if(size > 0){
                    for (int i=0; i<=size-1; i++){
                        cout << "id\t nazwa\t idKlienta" << endl;
                        Naprawa *utemp = new Naprawa;
                        utemp = &urzadzeniaKlienciTab[i];
                        //cout << "\t " << utemp->nazwa << "\t " << utemp->podajKlientId() << endl;
                        utemp->wyswietl();
                    }
                }else
                    cout << "brak\n\n";
                
                btn();
                break;
            }
                
            case 6:{
                cout << "Podaj ID klienta do edycji: ";
                cin >> menu;
                
                bool found = false;
                string data;
                
                auto it = klienciTab.begin();
                while(!found){
                    if((*it)->podajId() == menu){
                        (*it)->wypiszDane();
                        found = true;
                    }else
                        it++;
                    
                    if(it <= klienciTab.end()-1) break;
                    
                }
                
                
                if(found){
                    cout << "zmienic (1)imie (2)nazwisko czy (4)usun?\n";
                    cin >> menu;
                    
                    switch(menu){
                        default:break;
                        case 1:{
                            cin >> data;
                            (*it)->zmienImie(data);
                            (*it)->wypiszDane();
                            save();
                            break;
                        }
                        case 2:{
                            cin >> data;
                            (*it)->zmienNazwisko(data);
                            save();
                            break;
                        }
                            
                        case 4:{
                            klienciTab.erase(it);
                            save();
                            break;
                        }
                    }
                }
                
                break;
            }
   
            case 7:{
                cout << "Podaj ID klienta do edycji: ";
                cin >> menu;
                
                bool found = false;
                string data;
                
                auto it = klienciFirmaTab.begin();
                while(!found){
                    if((*it)->podajId() == menu){
                        (*it)->wypiszDane();
                        found = true;
                    }else
                        it++;
                    
                    if(it <= klienciFirmaTab.end()-1){
                        cout << "nie znaleziono klienta\n";
                        break;
                    }
                    
                }
                
                
                if(found){
                    cout << "zmienic (1)imie (2)nazwisko (3)firme czy (4)usun?\n";
                    cin >> menu;
                    
                    switch(menu){
                        default:break;
                        case 1:{
                            cin >> data;
                            (*it)->zmienImie(data);
                            (*it)->wypiszDane();
                            save();
                            break;
                        }
                        case 2:{
                            cin >> data;
                            (*it)->zmienNazwisko(data);
                            save();
                            break;
                        }
                        case 3:{
                            cin >> data;
                            (*it)->zmienFirme(data);
                            save();
                            break;
                        }
                            
                        case 4:{
                            klienciFirmaTab.erase(it);
                            save();
                            break;
                        }
                    }
                }
                
                break;
            }
                
            default:
                cout << "zla opcja!";
                btn();
                break;
        }
        
    }
    
    
    return 0;
    
    
}

