/*
 DHT11+ esp8266 
 
*/
#include <ESP8266WiFi.h>
#include <DHT.h>

//extern "C" {
//#include <user_interface.h>
//}

#define DHTPIN 4     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "";
const char* password = "";

const char* host = "your-web-jp";

static uint32_t mTimerTmp;
static uint32_t mTimerPost;
String mTemp="";
String mHum="";

//
void proc_http(String sTemp, String sHum){
//Serial.print("connecting to ");
//Serial.println(host);  
      WiFiClient client;
      const int httpPort = 80;
      if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      String url = "/update?field1="+ sTemp +"&field2=" + sHum;        
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
        "Host: " + host + "\r\n" + 
        "Connection: close\r\n\r\n");
      delay(10);      
      int iSt=0;
      while(client.available()){
          String line = client.readStringUntil('\r');
Serial.print(line);
      }    
}


//
void setup() {
  //pinMode(mLedPin, OUTPUT);
  Serial.begin( 9600 );
  Serial.println("#Start-setup");
  Serial.print("millis.Start: ");
  Serial.println(millis() );

  dht.begin();
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
      
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());

   //proc_http("11", "22");
}

void loop() {
  delay( 100 );
  if (millis() > mTimerTmp) {
      mTimerTmp = millis()+ 3000;
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
        return;
    }
    int itemp =(int)t;   
    int iHum = (int)h;   
    mTemp=String(itemp);          
    mHum=String(iHum);
Serial.print("dat="); 
Serial.print(mTemp);
Serial.println( mHum );
  }
  if (millis() > mTimerPost) {
      mTimerPost= millis()+ 60000;
      proc_http(mTemp, mHum);
      //delay(100);
  }
  
}


