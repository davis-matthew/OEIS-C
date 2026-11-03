/*
 * A017554  a(n) = (12*n + 2)^10.
 * https://oeis.org/A017554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+2)^10
 * Range:     u8 none, u16 n <= 0, u32 n <= 0, u64 n <= 6, u128 n <= 594,
 *            big any n
 * Validated: standard, pass, 30 terms through n = 9784 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)2;
    return ipow_i32(base, 10);
}
