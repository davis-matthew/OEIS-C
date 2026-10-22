/*
 * A007925  a(n) = n^(n+1) - (n+1)^n.
 * https://oeis.org/A007925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = n^(n+1) - (n+1)^n
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 26,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t term1 = a000272_ipow(n_seq, n + 1);
    seq_t term2 = a000272_ipow(n_seq + 1, n);
    return term1 - term2;
}
