/*
 * A127947  Hankel transform of central coefficients of (1+k*x+5x^2)^n, k
 *          arbitrary integer.
 * https://oeis.org/A127947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * 5^{n*(n+1)/2}
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* exponent for 5: n*(n+1)/2 */
    seq_t exp = ((seq_t)n * ((seq_t)n + 1)) / 2;

    /* compute 2^n and 5^exp using big_pow */
    seq_t term2 = big_pow((seq_t)2, n);
    seq_t term5 = big_pow((seq_t)5, exp);

    return term2 * term5;
}
