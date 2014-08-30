//===--- LangOptions.h - Language & configuration options -------*- C++ -*-===//
//
// This source file is part of the Swift.org open source project
//
// Copyright (c) 2014 - 2015 Apple Inc. and the Swift project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See http://swift.org/LICENSE.txt for license information
// See http://swift.org/CONTRIBUTORS.txt for the list of Swift project authors
//
//===----------------------------------------------------------------------===//
//
//  This file defines the LangOptions class, which provides various
//  language and configuration flags.
//
//===----------------------------------------------------------------------===//

#ifndef SWIFT_LANGOPTIONS_H
#define SWIFT_LANGOPTIONS_H

#include "swift/Basic/LLVM.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "clang/Basic/VersionTuple.h"
#include <string>

namespace swift {
  /// \brief A collection of options that affect the language dialect and
  /// provide compiler debugging facilities.
  class LangOptions {
  public:

    /// \brief Enable legacy character literals.
    bool EnableCharacterLiterals = false;
    
    /// \brief Whether we are debugging the constraint solver.
    ///
    /// This option enables verbose debugging output from the constraint
    /// solver.
    bool DebugConstraintSolver = false;

    /// \brief Specific solution attempt for which the constraint
    /// solver should be debugged.
    unsigned DebugConstraintSolverAttempt = 0;
    
    /// \brief The upper bound, in bytes, of temporary data that can be
    /// allocated by the constraint solver.
    unsigned SolverMemoryThreshold = 15000000;

    /// \brief Perform all dynamic allocations using malloc/free instead of
    /// optimized custom allocator, so that memory debugging tools can be used.
    bool UseMalloc = false;
    
    /// \brief Enable experimental "switch" pattern-matching features.
    bool EnableExperimentalPatterns = false;

    /// \brief Enable features useful for running in the debugger.
    bool DebuggerSupport = false;

    /// \brief Enable features useful for running playgrounds.
    // FIXME: This should probably be limited to the particular SourceFile.
    bool Playground = false;

    /// \brief Keep comments during lexing and attach them to declarations.
    bool AttachCommentsToDecls = false;

    /// Enable 'availability' restrictions for App Extensions.
    bool EnableAppExtensionRestrictions = false;

    /// Whether to split imported Objective-C selectors into Swift method names.
    bool SplitPrepositions = false;

    /// Enables checking that uses of @objc require importing
    /// the Foundation module.
    /// This is enabled by default because SILGen can crash in such a case, but
    /// it gets disabled when compiling the Swift core stdlib.
    bool EnableObjCAttrRequiresFoundation = true;

    /// Enables importing of unions and other partially-importable types.
    bool ImportUnions = false;
    
    /// Should access control be respected?
    bool EnableAccessControl = true;

    /// Triggers emission of an artificial 'main' for an AppKit project.
    bool EmitNSApplicationMain = false;

    /// If set, symbol mangling will include private discriminators.
    ///
    /// This option is for staging purposes only; it will soon go away.
    /// In particular, setting this will not actually change the mangler's
    /// behavior; it just affects what the compiler \em thinks will happen.
    bool UsePrivateDiscriminators = false;
    
    /// The minimum platform version to which code will be deployed.
    clang::VersionTuple MinPlatformVersion;

    /// Implicit target configuration options.  There are currently two
    ///   supported target configuration values:
    ///     os - The active os target (OSX or IOS)
    ///     arch - The active arch target (X64, I386, ARM, ARM64)
    void addTargetConfigOption(StringRef Name, StringRef Value) {
      assert(!Name.empty() && !Value.empty());
      TargetConfigOptions.push_back(std::make_pair(Name, Value));
    }

    /// Removes all configuration options added with addTargetConfigOption.
    void clearAllTargetConfigOptions() {
      TargetConfigOptions.clear();
    }
    
    /// Returns the value for the given target configuration or an empty string.
    StringRef getTargetConfigOption(StringRef Name);
    
    /// Explicit build configuration options, initialized via the '-D'
    /// compiler flag.
    void addBuildConfigOption(StringRef Name) {
      assert(!Name.empty());
      BuildConfigOptions.push_back(Name);
    }

    /// Determines if a given build configuration has been defined.
    bool hasBuildConfigOption(StringRef Name);

    ArrayRef<std::pair<std::string, std::string>>
        getTargetConfigOptions() const {
      return TargetConfigOptions;
    }

  private:
    llvm::SmallVector<std::pair<std::string, std::string>, 2>
        TargetConfigOptions; 
    llvm::SmallVector<std::string, 2> BuildConfigOptions; 
  };
}

#endif // LLVM_SWIFT_LANGOPTIONS_H

