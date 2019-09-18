
/*
Written By: Todd Murphy
*/

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid     = "TheHub";
const char* password = "sigmaalphaepsilon";

byte buttonPin = 2;
byte relayPin = 5; //used to be 14

//string to store response
  String passwordString;

  //store client.read
  char printer;

WiFiServer server ( 80 );

void setup()
{
  //----------------------
  // Pin Setup
  //----------------------
  pinMode(buttonPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(buttonPin, HIGH);
  digitalWrite(relayPin, LOW);

  unlockDoor();

  Serial.begin(9600);

  // Connecting to a WiFi network
   Serial.println();
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
   WiFi.begin(ssid, password);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   Serial.println("");
   Serial.println("WiFi connected");  
   Serial.println("IP address: ");
   Serial.println(WiFi.localIP());

  delay(2000);
  server.begin();

  Serial.println("setup finished");
  passwordString = "";
}

void loop()
{
  // listen for incoming clients
  WiFiClient client = server.available();
  
  //-----------------------
  // Button Override
  //-----------------------
  if (digitalRead(buttonPin)==LOW){
    unlockDoor();
  }
  
  if (client) {
    Serial.println("Client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      while(client.available()) {
        char c = client.read();
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

// Here is where the POST data is. 
          while(client.available())
          {
             printer = client.read();
             Serial.write(printer);
             passwordString += printer;
          }
          Serial.println();

          Serial.println("Sending response");
          // send a standard http response header
          client.println("HTTP/1.0 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          //client.println("<HTML><TITLE>Todd's Bathroom Lock</TITLE><BODY>Todd's Bathroom Door Lock");
         
         
//form added to send data from browser and view received data in serial monitor
client.println("<html><head><title>Todd's Bathroom Lock</title></head>");
client.println("<style>body{background-color: #597901;}table{border-style: solid;position: absolute;top: 20%;left: 40%;padding:10px;background-color: #cbcbcb;border-radius: 10px;}</style>");
client.println("<body><h1 style=\"position:absolute;left:39%;\">Todd's Bathroom Lock</h1><form method=\"post\" action=\"#\">");
client.println("<table><tr bgcolor=\"purple\"><th colspan=\"3\"><font color=\"white\">Enter the password</th></tr><tr height=\"20\"></tr><tr><td>Password: </td><td><input type=\"password\" name=\"password\"value=\"\" size=\"25\" maxlength=\"50\"/></td></tr><tr height=\"10\"></tr><tr><td></td><td></td><td align=\"center\"><input type=\"submit\" value=\"Unlock Door\"></td></tr></table>");
client.println("</form></body></html>");
//client.println("<DIV style=\"margain:auto;width:50%;\"><FORM ACTION=\"http://192.168.1.16:80\" METHOD=\"post\">");
//client.println("Password: <INPUT TYPE=\"PASSWORD\" NAME=\"Name\" VALUE=\"\" SIZE=\"25\" MAXLENGTH=\"50\"><BR>");
//client.println("<INPUT TYPE=\"SUBMIT\" NAME=\"submit\" VALUE=\"Open Door\">");
//client.println("</FORM></DIV>");
//client.println("<BR>");
         
          client.stop();
        }
        else if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }

    //startPass = passwordString.indexOf("=");
    //endPass = passwordString.indexOf("&");

    //password to String
    
    //this will get just the password, no other garbage
    passwordString = passwordString.substring((passwordString.indexOf("=")+1), passwordString.indexOf("&"));

    //----------------------------------------------------------
    // Main Password Check
    //----------------------------------------------------------
    if (passwordString.equals("13255")){
      unlockDoor();
    }
    
    Serial.println("Disconnected");
    passwordString = "";

    //-----------------------
    // Button Override
    //-----------------------
    if (digitalRead(buttonPin)==LOW){
      unlockDoor();
    }
  }
}

void unlockDoor(){
  digitalWrite(relayPin, HIGH);
  delay(1000);
  digitalWrite(relayPin, LOW);
  delay(5000); //open for 5 seconds
  digitalWrite(relayPin, HIGH);
  delay(1000);
  digitalWrite(relayPin, LOW);
}
