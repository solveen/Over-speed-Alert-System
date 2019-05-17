#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Wire.h>

char msg;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;


TinyGPSPlus gps;
SoftwareSerial ss(TXPin, RXPin);
SoftwareSerial mySerial(9, 10);

float velocity = 0;

void setup(){
  Serial.begin(9600);
  ss.begin(GPSBaud);

  Serial.println("ADXL345 Accelerometer Calibration"); 
  Serial.println("");

  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
}

void loop(){

  while (ss.available() > 0){
    gps.encode(ss.read());
    if (gps.location.isUpdated()){
      
  
    double latitude=(gps.location.lat(),6);
    float longitude=(gps.location.lng());
    float altitude=(gps.altitude.meters());
    String latti;
    latti=String(gps.location.lat(), 6);
    String lng;
    lng=String(gps.location.lng(), 6);
  


 delay(1000);
    /* Get a new sensor event */ 
    sensors_event_t accelEvent;  
    accel.getEvent(&accelEvent);
    
    Serial.print("x :");
    velocity = accelEvent.acceleration.x;
    Serial.println(velocity);

    if(velocity>16.67)//16.67 m/s = 60km/hr 
    {
    Serial.println("Over speeding");
    Serial.println(latti);
    Serial.println(lng);   
        if (mySerial.available()>0)
              Serial.write(mySerial.read());
    
        while (Serial.available())
        {
          
          Serial.read();
    
            
          // clear the input buffer
        }
        

    mySerial.begin(9600);
    Serial.println("Sending Message");
    mySerial.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
    delay(1000);  // Delay of 1000 milli seconds or 1 second
    mySerial.println("AT+CMGS=\"+9779823082248\"\r"); // Replace x with mobile number
    delay(1000);
    mySerial.println("Your child is overspeeding");
    mySerial.println("Your child's location is Latitude: ");
    mySerial.println(latti);
    mySerial.println("and Longitude: ");
     mySerial.println("Please take some action soon.. or your child will die ");
    mySerial.println(lng);// The SMS text you want to send
    delay(100);
     mySerial.println((char)26);// ASCII code of CTRL+Z
    delay(1000);
    ss.begin(GPSBaud);
    delay(5000);
      
    }
    while (Serial.available())
    {
      Serial.read();  // clear the input buffer
    }
    }}}
