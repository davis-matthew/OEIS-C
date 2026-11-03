/*
 * A009977  Powers of 33.
 * https://oeis.org/A009977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 33^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* Compute 33 to the power of n.
       The pow_uint utility handles arbitrary‑precision
       or built‑in integer types and returns the exact
       result for any non‑negative exponent. */
    return ipow_u64_idx(33, n);
}
