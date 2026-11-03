/*
 * A024099  a(n) = 8^n - n^11.
 * https://oeis.org/A024099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 8^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 21, i128 n <= 42,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 337 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    seq_t pow8   = ipow_u64_idx(8, n);
    seq_t pow11  = ipow_u64_idx(n, 11);
    return pow8 - pow11;
}
