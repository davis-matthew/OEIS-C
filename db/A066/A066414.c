/*
 * A066414  Numerators of coefficients in series expansion of
 *          -512*(1+x)^3/(x-8)^3.
 * https://oeis.org/A066414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A066414:  Numerators of the coefficients in the power‑series
    expansion of -512*(1+x)^3/(x-8)^3.  The function below computes
    the reduced numerator a(n) for n ≥ 0 as a(seq_t).  For the series

           (1+x)^3 * (1 - x/8)^(-3)

    the general coefficient is

           c_n = Σ_{j=0..3} binom(3,j) * binom(n-j+2,2) / 8^(n-j)

    where 8^k = 2^(3k).  The denominator of c_n is 2^(3n); the
    greatest power of two dividing the raw numerator is cancelled
    from both numerator and denominator.  The returned value is the
    numerator of c_n in lowest terms.
*/

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* Compute the raw numerator: Σ binom(3,j)*binom(n-j+2,2)*8^j  (j=0..3, j≤n) */
    seq_t num = 0;
    for (idx_t j = 0; j <= 3 && j <= n; ++j)
    {
        seq_t t = binom(3, j);
        t *= binom(n - j + 2, 2);      /* binom(n-j+2,2) */
        t <<= (3 * j);                 /* multiply by 8^j = 2^(3j) */
        num += t;
    }

    /* Cancel common powers of 2 with the denominator 2^(3n). */
    idx_t max_shift = 3 * n;           /* denominator exponent */
    idx_t shift = 0;
    while (shift < max_shift && (num & 1) == 0)
    {
        num >>= 1;
        ++shift;
    }

    return num;                        /* reduced numerator a(n) */
}
