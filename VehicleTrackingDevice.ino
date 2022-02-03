//Realtime GPS Tracker with Nodemcu ESP8266
#define BLYNK_TEMPLATE_ID ""
#define BLYNK_DEVICE_NAME ""
#define BLYNK_AUTH_TOKEN ""

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


static const int RXPin = 4, TXPin = 5;   // 
static const uint32_t GPSBaud = 9600; 

TinyGPSPlus gps; // The TinyGPS++ object


SoftwareSerial ss(RXPin, TXPin);  // The serial connection to the GPS device

BlynkTimer timer;

float spd;       //Variable  to store the speed
float sats;      //Variable to store no. of satellites response
String bearing;  //Variable to store orientation or direction of GPS

char auth[] = "";              //Your Project authentication key
char ssid[] = "ok";                                       // Name of your network (HotSpot or Router name)
char pass[] = "";                                      // Corresponding Password

//unsigned int move_index;         // moving index, to be used later
unsigned int move_index = 1;       // fixed location for now
  

void setup()
{
  Serial.begin(9600);
  WiFi.begin("ok", "");
  
  while(WiFi.status() !=WL_CONNECTED)
  {   delay(500);
      Serial.print(".");
  }
  Serial.println();
  Serial.println("NodeMCU is Connected");
  Serial.println(WiFi.localIP());
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(500L, checkGPS); // every 5s check if GPS is connected, only really needs to be done once
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
      Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
     
  }
}

void loop()
{
   while(ss.available() >0){
      // sketch displays information every time a new sentence is correctly encoded.
      if (gps.encode(ss.read())){
        displayInfo();
      }
   }
  Blynk.run();
  timer.run();
}

void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    double latitude = (gps.location.lat());     //Storing the Lat. and Lon. 
    double longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6);  // float to x decimal places
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, (latitude, 6));   
    Blynk.virtualWrite(V2, (longitude, 6));  
    
    spd = gps.speed.kmph();               //get speed
       Blynk.virtualWrite(V3, spd);
    
                       
  }
  
}
