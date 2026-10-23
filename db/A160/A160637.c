/*
 * A160637  Hankel transform of A114464(n+1).
 * https://oeis.org/A160637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-2)^floor(binomial(n+1,2)/2)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 10, i64 n <= 15, i128 n <= 22,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent = floor((n*(n+1))/4) */
    seq_t exponent = ((seq_t)n * ((seq_t)n + 1)) / 4;
    seq_t value     = big_pow((seq_t)2, exponent);
    /* apply the sign: (-1)^exponent */
    if ((exponent & 1) != 0) {
        value = -value;
    }
    return value;
}
