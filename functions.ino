void configNetwork() {
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("WiFi shield not present!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:

  while ( status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    Serial.print ( "." );
    // Wait for connection
    delay ( 500 );
  }

#ifdef DEBUG
  // you're connected now, so print out the status:
  printWiFiStatus();
#endif

}

// Sets internal RTC from external NTP source

void configRTC () {
  rtc.begin();

  unsigned long epoch;
  int numberOfTries = 0, maxTries = 10;
  do {
    epoch = WiFi.getTime();
    numberOfTries++;
    Serial.print("n");
    delay(2000);
  }
  while ((epoch == 0) && (numberOfTries < maxTries));

  if (numberOfTries > maxTries) {
    Serial.print("NTP unreachable!!");
    while (1);
  }
  else {
    rtc.setEpoch(epoch);
#ifdef DEBUG
    Serial.print("Epoch received: ");
    Serial.println(epoch);
#endif
  }
}

void printWiFiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void getBoardID(char board_id[])
{
  // Gets board MAC address
  byte mac[ MAC_LENGTH ];           // Holds board MAC address
  WiFi.macAddress(mac);
  array_to_string(mac, 6, board_id);
}

// Convert MAC address to its corresponding HEX string
void array_to_string(byte array[], unsigned int len, char buffer[])
{
  for (unsigned int i = 0; i < len; i++)
  {
    byte nib1 = (array[len - i - 1] >> 4) & 0x0F;
    byte nib2 = (array[len - i - 1] >> 0) & 0x0F;
    buffer[i * 2 + 0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
    buffer[i * 2 + 1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
  }
  buffer[len * 2] = '\0';
}