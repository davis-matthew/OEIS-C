/*
 * A088802  Denominators of the coefficients of powers of n^(-1) in the
 *          Romanovsky series expansion of the mean of the standard deviation
 *          from a normal population.
 * https://oeis.org/A088802
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator(binomial(1/4,n)) = 2^(2n + v₂(n!)) where
 *            v₂(m) = Σ floor(m / 2^k) for k≥1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>               /* only for completeness */

seq_t a(idx_t n)
{
    /* compute v₂(n!) via successive division by 2 */
    idx_t half = n >> 1;          /* floor(n/2) */
    idx_t v2   = 0;
    while (half) {
        v2 += half;
        half >>= 1;               /* divide by 2 again */
    }

    /* exponent of 2 in the denominator */
    idx_t exp = (n << 1) + v2;    /* 2n + v₂(n!) */

    /* compute 2^exp */
    seq_t result = (seq_t)1;
    while (exp--) {
        result <<= 1;             /* shift left by 1; safe for any integer type */
    }
    return result;
}
