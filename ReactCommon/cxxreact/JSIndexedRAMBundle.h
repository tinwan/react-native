// Copyright 2004-present Facebook. All Rights Reserved.

#pragma once

#include <fstream>
#include <memory>

#include "Executor.h"
#include "JSBundleType.h"

namespace facebook {
namespace react {

class JSBigString;

#include <cxxreact/JSModulesUnbundle.h>

class JSIndexedRAMBundle : public facebook::react::JSModulesUnbundle {
public:
  // Throws std::runtime_error on failure.
  JSIndexedRAMBundle(const char *sourceURL);

  // Throws std::runtime_error on failure.
  std::unique_ptr<const facebook::react::JSBigString> getStartupCode();
  // Throws std::runtime_error on failure.
  Module getModule(uint32_t moduleId) const override;

private:
  struct ModuleData {
    uint32_t offset;
    uint32_t length;
  };
  static_assert(
    sizeof(ModuleData) == 8,
    "ModuleData must not have any padding and use sizes matching input files");

  struct ModuleTable {
    size_t numEntries;
    std::unique_ptr<ModuleData[]> data;
    ModuleTable() : numEntries(0) {};
    ModuleTable(size_t entries) :
      numEntries(entries),
      data(std::make_unique<ModuleData[]>(numEntries)) {};
    size_t byteLength() const {
      return numEntries * sizeof(ModuleData);
    }
  };

  std::string getModuleCode(const uint32_t id) const;
  void readBundle(char *buffer, const std::streamsize bytes) const;
  void readBundle(
    char *buffer, const
    std::streamsize bytes,
    const std::ifstream::pos_type position) const;

  mutable std::ifstream m_bundle;
  ModuleTable m_table;
  size_t m_baseOffset;
  std::unique_ptr<facebook::react::JSBigBufferString> m_startupCode;
};

}  // namespace react
}  // namespace facebook
