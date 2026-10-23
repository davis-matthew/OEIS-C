/*
 * A024029  a(n) = 3^n - n^6.
 * https://oeis.org/A024029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3^n - n^6
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 19, i64 n <= 39, i128 n <= 80,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t once */
    seq_t sn = (seq_t)n;

    /* compute 3^n */
    seq_t pow3 = big_pow((seq_t)3, sn);

    /* compute n^6 = (n^2) * (n^4) */
    seq_t n_sq = sn * sn;
    seq_t n4  = n_sq * n_sq;
    seq_t n6  = n_sq * n4;

    return pow3 - n6;
}
