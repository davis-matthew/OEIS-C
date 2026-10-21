/*
 * A099377  Numerators of the harmonic means of the divisors of the positive
 *          integers.
 * https://oeis.org/A099377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n * τ(n) / σ(n) reduced to simplest fraction, numerator
 *            only where τ(n) is the number of divisors and σ(n) is their sum
 * Range:     u8 n <= 35, u16 n <= 3599, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;     /* n as seq_t */
    seq_t tau = 1;           /* divisor count */
    seq_t sigma = 1;         /* divisor sum */

    idx_t m = n;              /* remaining unfactored part of n */
    idx_t p = 2;              /* current trial divisor */

    /* Factor n by trial division and update tau & sigma */
    while (p <= m / p) {
        if (m % p == 0) {
            idx_t e = 0;           /* exponent of prime p */
            seq_t p_pow = 1;       /* p^e as seq_t */

            while (m % p == 0) {
                m /= p;
                ++e;
                p_pow *= (seq_t)p;
            }

            tau *= (seq_t)(e + 1);

            /* σ(p^e) = (p^(e+1) – 1) / (p – 1) */
            seq_t top = p_pow * (seq_t)p - (seq_t)1;
            seq_t denom = (seq_t)p - (seq_t)1;
            sigma *= top / denom;
        }
        ++p;
    }

    /* If a prime factor greater than sqrt(n) remains */
    if (m > 1) {
        tau *= (seq_t)2;                /* exponent 1 contributes factor (1+1) */
        sigma *= ((seq_t)m + (seq_t)1); /* σ(p) = p + 1 */
    }

    /* Numerator of harmonic mean: n * τ(n) %
     * Reduce by gcd with σ(n) to keep the fraction simplest */
    seq_t num = nn * tau;
    seq_t g = gcd(num, sigma);
    return num / g;
}
