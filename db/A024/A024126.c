/*
 * A024126  a(n) = 10^n - n^12.
 * https://oeis.org/A024126
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 10^n - n^12
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 18, i128 n <= 38,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 298 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t p10 = ipow_u64_idx(10, n);
    seq_t n12 = ipow_u64_idx(n, 12);
    return p10 - n12;
}
