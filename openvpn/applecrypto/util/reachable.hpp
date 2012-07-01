#ifndef OPENVPN_APPLECRYPTO_UTIL_REACHABLE_H
#define OPENVPN_APPLECRYPTO_UTIL_REACHABLE_H

#include <netinet/in.h>
#include <SystemConfiguration/SCNetworkReachability.h>

#include <string>
#include <sstream>

namespace openvpn {
  // Helper class for determining network reachability
  class Reachability
  {
  public:
    Reachability()
      : didRetrieveFlags(FALSE),
	flags(0)
    {
      struct sockaddr_in zeroAddress;
      bzero(&zeroAddress, sizeof(zeroAddress));
      zeroAddress.sin_len = sizeof(zeroAddress);
      zeroAddress.sin_family = AF_INET;
      SCNetworkReachabilityRef defaultRouteReachability = SCNetworkReachabilityCreateWithAddress(kCFAllocatorDefault, (struct sockaddr*)&zeroAddress);
      didRetrieveFlags = SCNetworkReachabilityGetFlags(defaultRouteReachability, &flags);
      CFRelease(defaultRouteReachability);
    }

    bool defined() const { return bool(didRetrieveFlags); }
    bool reachable() const { return bool(flags & kSCNetworkReachabilityFlagsReachable); }
    bool connectionRequired() const { return bool(flags & kSCNetworkReachabilityFlagsConnectionRequired); }
    bool isWWAN() const { return bool(flags & kSCNetworkReachabilityFlagsIsWWAN); }

    std::string to_string() const {
      std::ostringstream out;
      out << "Reachability<";
      if (defined())
	{
	  out << "reachable=" << reachable();
	  out << " connectionRequired=" << connectionRequired();
	  out << " isWWAN=" << isWWAN();
	}
      else
	out << "UNDEF";
      out << '>';
      return out.str();
    }

  private:
    Boolean didRetrieveFlags;
    SCNetworkReachabilityFlags flags;
  };

}

#endif
