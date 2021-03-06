#include <iostream>
#include <memory>

#include "baldr/streetnames_us.h"
#include "midgard/util.h"

namespace valhalla {
namespace baldr {

StreetNamesUs::StreetNamesUs() : StreetNames() {
}

StreetNamesUs::StreetNamesUs(const std::vector<std::string>& names) {
  for (auto& name : names) {
    this->emplace_back(midgard::make_unique<StreetNameUs>(name));
  }
}

StreetNamesUs::~StreetNamesUs() {
}

std::unique_ptr<StreetNames> StreetNamesUs::clone() const {
  std::unique_ptr<StreetNames> clone_street_names = midgard::make_unique<StreetNamesUs>();
  for (const auto& street_name : *this) {
    clone_street_names->emplace_back(midgard::make_unique<StreetNameUs>(street_name->value()));
  }

  return clone_street_names;
}

std::unique_ptr<StreetNames>
StreetNamesUs::FindCommonStreetNames(const StreetNames& other_street_names) const {
  std::unique_ptr<StreetNames> common_street_names = midgard::make_unique<StreetNamesUs>();
  for (const auto& street_name : *this) {
    for (const auto& other_street_name : other_street_names) {
      if (*street_name == *other_street_name) {
        common_street_names->emplace_back(midgard::make_unique<StreetNameUs>(street_name->value()));
        break;
      }
    }
  }

  return common_street_names;
}

std::unique_ptr<StreetNames>
StreetNamesUs::FindCommonBaseNames(const StreetNames& other_street_names) const {
  std::unique_ptr<StreetNames> common_base_names = midgard::make_unique<StreetNamesUs>();
  for (const auto& street_name : *this) {
    for (const auto& other_street_name : other_street_names) {
      if (street_name->HasSameBaseName(*other_street_name)) {
        // Use the name with the cardinal directional suffix
        // thus, 'US 30 West' will be used instead of 'US 30'
        if (!street_name->GetPostCardinalDir().empty()) {
          common_base_names->emplace_back(midgard::make_unique<StreetNameUs>(street_name->value()));
        } else if (!other_street_name->GetPostCardinalDir().empty()) {
          common_base_names->emplace_back(
              midgard::make_unique<StreetNameUs>(other_street_name->value()));
          // Use street_name by default
        } else {
          common_base_names->emplace_back(midgard::make_unique<StreetNameUs>(street_name->value()));
        }
        break;
      }
    }
  }

  return common_base_names;
}

} // namespace baldr
} // namespace valhalla
