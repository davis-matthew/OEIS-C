/*
 * A168298  a(n) = 1 - n^2*2^n.
 * https://oeis.org/A168298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - n^2*2^n
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 22, i64 n <= 51, i128 n <= 113,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t nn   = (seq_t)n;
    seq_t n_sq = nn * nn;
    seq_t prod = n_sq * pow2(n);
    return (seq_t)1 - prod;
}
