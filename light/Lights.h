/*
 * SPDX-FileCopyrightText: 2018-2024 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <aidl/android/hardware/light/BnLights.h>

#include <unordered_map>

using ::aidl::android::hardware::light::BnLights;
using ::aidl::android::hardware::light::FlashMode;
using ::aidl::android::hardware::light::HwLight;
using ::aidl::android::hardware::light::HwLightState;
using ::aidl::android::hardware::light::LightType;

namespace aidl {
namespace android {
namespace hardware {
namespace light {

class Lights : public BnLights {
  public:
    Lights();

    ndk::ScopedAStatus setLightState(int id, const HwLightState& state) override;
    ndk::ScopedAStatus getLights(std::vector<HwLight>* types) override;

  private:
    void setLightBacklight(LightType type, const HwLightState& state);
    void setLightButtons(LightType type, const HwLightState& state);
    void setLightNotification(LightType type, const HwLightState& state);
    void applyNotificationState(const HwLightState& state);

    uint32_t max_button_brightness_;
    uint32_t max_led_brightness_;
    uint32_t max_screen_brightness_;

    std::unordered_map<LightType, std::function<void(LightType type, const HwLightState&)>> lights_{
            {LightType::BACKLIGHT, [this](auto&&... args) { setLightBacklight(args...); }},
            {LightType::BATTERY, [this](auto&&... args) { setLightNotification(args...); }},
            {LightType::NOTIFICATIONS, [this](auto&&... args) { setLightNotification(args...); }}};

    // Keep sorted in the order of importance.
    std::array<std::pair<LightType, HwLightState>, 2> notif_states_ = {{
            {LightType::NOTIFICATIONS, {}},
            {LightType::BATTERY, {}},
    }};

    std::vector<std::string> buttons_;
};

}  // namespace light
}  // namespace hardware
}  // namespace android
}  // namespace aidl
