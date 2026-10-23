/*
 * A024014  a(n) = 2^n-n^4.
 * https://oeis.org/A024014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2^n - n^4
 * Range:     i8 n <= 3, i16 n <= 16, i32 n <= 31, i64 n <= 63, i128 n <= 127,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2 = pow_int((seq_t)2, n);

    /* n^4 */
    seq_t n_seq = (seq_t)n;
    seq_t n_sq   = n_seq * n_seq;
    seq_t n4     = n_sq * n_sq;

    return pow2 - n4;
}
