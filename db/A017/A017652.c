/*
 * A017652  a(n) = (12*n + 10)^12.
 * https://oeis.org/A017652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 10)^12
 * Range:     u8 none, u16 none, u32 none, u64 n <= 2, u128 n <= 134,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 9839 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n * 12 + 10;
    return ipow_i32(base, 12);
}
