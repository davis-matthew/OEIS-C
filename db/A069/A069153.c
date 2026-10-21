/*
 * A069153  a(n) = Sum_{d|n} d*(d-1)/2.
 * https://oeis.org/A069153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{d|n} d*(d-1)/2
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to a(n) = (sigma2(n) - sigma1(n))/2, where sigma_k is the sum of the k-th powers of the divisors */

#include <stdbool.h>

static seq_t sqr(seq_t x) { return x * x; }

seq_t a(idx_t n)
{
    /* sigma1 and sigma2 are set to 1 initially */
    seq_t sigma1 = 1, sigma2 = 1;
    idx_t m = n;

    /* handle factor 2 separately */
    if ((m & 1) == 0) {
        idx_t e = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            ++e;
        }
        /* compute p^(e+1) and p^(2e+2) for p=2 */
        seq_t pPowE1 = 1;
        for (idx_t i = 0; i < e + 1; ++i) pPowE1 *= 2;
        seq_t pPowE2 = 1;
        for (idx_t i = 0; i < 2 * e + 2; ++i) pPowE2 *= 2;
        seq_t term1 = (pPowE1 - 1) / (2 - 1);   /* (2^(e+1)-1)/(2-1) = 2^(e+1)-1 */
        seq_t term2 = (pPowE2 - 1) / (4 - 1);   /* (2^(2e+2)-1)/(4-1) */
        sigma1 *= term1;
        sigma2 *= term2;
    }

    /* factor odd primes up to sqrt(m) */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* compute p^(e+1) and p^(2e+2) */
            seq_t pPowE1 = 1;
            for (idx_t i = 0; i < e + 1; ++i) pPowE1 *= (seq_t)p;
            seq_t pPowE2 = 1;
            for (idx_t i = 0; i < 2 * e + 2; ++i) pPowE2 *= (seq_t)p;
            seq_t term1 = (pPowE1 - 1) / ((seq_t)p - 1);
            seq_t term2 = (pPowE2 - 1) / (((seq_t)p * (seq_t)p) - 1);
            sigma1 *= term1;
            sigma2 *= term2;
        }
    }

    /* if a prime factor remains >1, it is prime */
    if (m > 1) {
        seq_t p = (seq_t)m;
        seq_t term1 = p + 1;           /* p^2-1 / (p-1) */
        seq_t term2 = sqr(p) + 1;      /* p^4-1 / (p^2-1) */
        sigma1 *= term1;
        sigma2 *= term2;
    }

    /* a(n) = (sigma2 - sigma1)/2 */
    return (sigma2 - sigma1) / 2;
}
