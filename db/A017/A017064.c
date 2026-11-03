/*
 * A017064  a(n) = (7*n+6)^12.
 * https://oeis.org/A017064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n+6)^12
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 4, u128 n <= 231,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 4400 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)7 * n + 6;
    return ipow_i32(base, 12);
}
