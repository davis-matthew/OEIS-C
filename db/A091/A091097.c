/*
 * A091097  Expansion of (1+6x-60x^2)/((1-6x)(1+6x)).
 * https://oeis.org/A091097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1; for n > 0, a(n) = 6^n if n is odd a(n) = -(2/3) * 6^n
 *            if n is even
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 11, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 6^n as a big integer. */
    seq_t pow6 = big_pow((seq_t)6, (seq_t)n);

    /* If n is odd, a(n) = 6^n. */
    if (n & 1)
        return pow6;

    /* If n is even, a(n) = - (2 * 6^n) / 3. */
    seq_t temp = pow6 * (seq_t)2;
    temp /= (seq_t)3;
    return -temp;
}
