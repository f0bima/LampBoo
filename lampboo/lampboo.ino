#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <FirebaseArduino.h>
#include "Setting.h"
#include "web.h"
#include <EEPROM.h>
 
void write_String(char add,String data);
String read_String(char add);

ESP8266WebServer server(80);
IPAddress ip(11, 3, 20, 19);
IPAddress gateway(11, 3, 20, 1);
IPAddress subnet(255, 255, 255, 0);

void setup() { 
  Serial.begin(9600);
  EEPROM.begin(512);

  if(setEEPROM){
    write_String(ssid_add, "SSID");
    write_String(pass_add, "PASSWORD");
    write_String(pb_add, "PROJECT BUCKET");
  }
  
  pinMode(relay, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(led, 1);
  digitalWrite(relay, 0);

  WiFi.mode(WIFI_AP_STA);      
  WiFi.begin(string2char(read_String(ssid_add)), string2char(read_String(pass_add))); 
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
  server.on("/", handleRoot);  
  server.on("/login", handleLogin);
  server.on("/setting", handleSetting);  
  server.on("/pb_set", handlePb_set);  
  server.on("/projectbucket", handleProjectbucket);  
  server.on("/style.css", handleCss);  
    
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);  
  server.collectHeaders(headerkeys, headerkeyssize );
  
  server.begin();
  WiFi.softAPConfig(ip, gateway, subnet);  
  WiFi.softAP(ssid, pass);
}


void loop() {
  if(WiFi.status() != WL_CONNECTED) {
    pinMode(led, 1);
    delay(500);
    pinMode(led, 0);
    delay(500);
  }
  else{  
    pinMode(led, 0);
    String pb_str = read_String(pb_add) + "/status";
    int stt = Firebase.getString("LampBoo/status").toInt();
    Serial.print("number: ");
    Serial.println(Firebase.getString("status"));
    digitalWrite(relay, stt);
  }  
  server.handleClient();
  delay(100);  
}
