/*
 * A009964  Powers of 20.
 * https://oeis.org/A009964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 88 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute 20^n using the provided big integer power helper. */
    return ipow_u64_idx(20ULL, n);
}
