/*
 * A017290  a(n) = (10*n + 1)^10.
 * https://oeis.org/A017290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 1)^10
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 8, u128 n <= 713,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 8932 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n * (seq_t)10 + (seq_t)1;
    return ipow_i32(base, 10);
}
