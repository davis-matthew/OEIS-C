/*
 * A017621  a(n) = (12*n + 8)^5.
 * https://oeis.org/A017621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 8)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 593, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 9634 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n * 12 + 8;
    return ipow_i32(val, 5);
}
