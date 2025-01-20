// Copyright Cartesi and individual authors (see AUTHORS)
// SPDX-License-Identifier: LGPL-3.0-or-later
//
// This program is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option) any
// later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License along
// with this program (see COPYING). If not, see <https://www.gnu.org/licenses/>.
//

#ifndef SHADOW_PMAS_H
#define SHADOW_PMAS_H

#include <cassert>
#include <cstdint>

#include "compiler-defines.h"
#include "pma-constants.h"
#include "pma-driver.h"

/// \file
/// \brief Shadow device.

namespace cartesi {

/// \brief Shadow memory layout

struct PACKED shadow_pma_entry {
    uint64_t istart;
    uint64_t ilength;
};

struct PACKED shadow_pmas_state {
    shadow_pma_entry pmas[PMA_MAX];
};

/// \brief List of field types
enum class shadow_pmas_what : uint64_t {
    istart = offsetof(shadow_pma_entry, istart),
    ilength = offsetof(shadow_pma_entry, ilength),
    unknown_
};

/// \brief Obtains the absolute address of a PMA entry in shadow memory.
/// \param p Index of desired shadow PMA entry
/// \returns The address.
static constexpr uint64_t shadow_pmas_get_pma_abs_addr(uint64_t p) {
    return PMA_SHADOW_PMAS_START + p * sizeof(shadow_pma_entry);
}

/// \brief Obtains the absolute address of a PMA entry in shadow memory.
/// \param p Index of desired shadow PMA entry
/// \param what Desired field
/// \returns The address.
static constexpr uint64_t shadow_pmas_get_pma_abs_addr(uint64_t p, shadow_pmas_what what) {
    return shadow_pmas_get_pma_abs_addr(p) + static_cast<uint64_t>(what);
}

static constexpr shadow_pmas_what shadow_pmas_get_what(uint64_t paddr) {
    if (paddr < PMA_SHADOW_PMAS_START || paddr - PMA_SHADOW_PMAS_START >= sizeof(shadow_pmas_state) ||
        (paddr & (sizeof(uint64_t) - 1)) != 0) {
        return shadow_pmas_what::unknown_;
    }
    paddr -= PMA_SHADOW_PMAS_START;
    return shadow_pmas_what{paddr % sizeof(shadow_pma_entry)};
}

static constexpr const char *shadow_pmas_get_what_name(shadow_pmas_what what) {
    const auto paddr = static_cast<uint64_t>(what);
    if (paddr < PMA_SHADOW_PMAS_START || paddr - PMA_SHADOW_PMAS_START >= sizeof(shadow_pmas_state) ||
        (paddr & (sizeof(uint64_t) - 1)) != 0) {
        return "pma.unknown";
    }
    using reg = shadow_pmas_what;
    switch (what) {
        case reg::istart:
            return "pma.istart";
        case reg::ilength:
            return "pma.ilength";
        case reg::unknown_:
            return "pma.unknown";
    }
}

} // namespace cartesi

#endif
