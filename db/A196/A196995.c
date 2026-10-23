/*
 * A196995  Determinant of Killing form K(x,y) of the Lie algebra sl(n,C) for
 *          n >=1.
 * https://oeis.org/A196995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^binomial(n,2) * 2^(n^2-1) * n^(n^2) for n>=2, a(1)=0
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 3, i64 n <= 4, i128 n <= 5,
 *            big any n
 * Helpers:   big_pow, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)0;

    /* binomial(n,2) */
    seq_t k = (seq_t)n * ((seq_t)n - 1) / (seq_t)2;
    seq_t sign = (seq_t)1;
    if ((k & (seq_t)1) != 0)
        sign = (seq_t)-1;

    /* exponent e = n^2 */
    seq_t e = (seq_t)n * (seq_t)n;

    /* n^(n^2) */
    seq_t pow_n = big_pow((seq_t)n, e);

    /* 2^(n^2-1) */
    seq_t pow2 = big_pow((seq_t)2, e - (seq_t)1);

    /* product */
    seq_t prod = seq_mul(pow_n, pow2);

    /* apply sign */
    return seq_mul(prod, sign);
}
