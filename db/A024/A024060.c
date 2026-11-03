/*
 * A024060  a(n) = 5^n - n^11.
 * https://oeis.org/A024060
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5^n - n^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 27, i128 n <= 54,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 488 (2026-11-01)
 * Helpers:   ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n) {
    seq_t five_pow = ipow_u64_idx(5, n);
    seq_t n_pow = ipow_u64_idx((uint64_t)n, 11);
    return five_pow - n_pow;
}
