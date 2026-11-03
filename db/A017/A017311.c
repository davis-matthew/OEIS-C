/*
 * A017311  a(n) = (10*n + 3)^7.
 * https://oeis.org/A017311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 56, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 9377 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    /* Compute 10*n + 3 in seq_t to avoid overflow of idx_t */
    seq_t base = ((seq_t)n) * 10 + 3;
    /* Raise to the 7th power using the provided helper */
    return ipow_i32(base, 7);
}
