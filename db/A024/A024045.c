/*
 * A024045  a(n) = 4^n-n^9.
 * https://oeis.org/A024045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^n - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 31, i128 n <= 63,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 498 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t p4 = pow4(n);
    seq_t p9 = ipow_idx((seq_t)n, 9);
    return p4 - p9;
}
