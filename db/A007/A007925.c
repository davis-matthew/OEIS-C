/*
 * A007925  a(n) = n^(n+1) - (n+1)^n.
 * https://oeis.org/A007925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = n^(n+1) - (n+1)^n
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 95 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ipow_idx.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t term1 = ipow_idx(n_seq, n + 1);
    seq_t term2 = ipow_idx(n_seq + 1, n);
    return term1 - term2;
}
