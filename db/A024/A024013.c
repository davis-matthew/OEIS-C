/*
 * A024013  a(n) = 2^n - n^3.
 * https://oeis.org/A024013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^3
 * Range:     i8 n <= 5, i16 n <= 15, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = pow2(n) - (n*n*n) using the utility function pow2 */
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    seq_t pow = pow2(n);
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn * nn;
    return pow - cube;
}
