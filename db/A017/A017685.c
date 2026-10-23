/*
 * A017685  Numerator of sum of -11th powers of divisors of n.
 * https://oeis.org/A017685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of sum_{d|n} 1/d^11 a(n) = sigma_11(n) /
 *            gcd(sigma_11(n), n^11) where sigma_11 is the usual divisor sum
 *            with exponent 11. The implementation factors n, builds sigma_11
 *            multiplicatively, computes the gcd with n^11 using prime‑factor
 *            powers, and finally divides. All arithmetic is p...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 3185,
 *            big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/pow_uint.h"   /* pow_uint(uint64_t base, idx_t exp) -> seq_t */
#include "../utils/factor_u64.h" /* factor_u64(uint64_t n, uint64_t *primes,
                                  uint64_t *exps, size_t *cnt) */

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents.  */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    if (n > 1)
        factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute sigma_11(n) = product over primes of (1 + p^11 + ... + p^{11e}). */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t p_11 = pow_uint(p, 11);     /* p^11  */
        seq_t sum = 0;
        seq_t term = 1;
        for (uint64_t j = 0; j <= e; ++j) {
            sum += term;
            term *= p_11;                /* term = p^{11(j+1)} */
        }
        sigma *= sum;
    }

    /* Compute G = gcd(sigma, n^11) by extracting prime powers. */
    seq_t g = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = primes[i];
        uint64_t e = exps[i];

        /* Count how many powers of q divide sigma.  */
        seq_t temp = sigma;
        unsigned int v = 0;
        while (temp % (seq_t)q == 0) {
            temp /= q;
            ++v;
        }
        /* The exponent in n^11 is 11*e.  */
        if (v > 11 * e)
            v = 11 * e;

        /* Multiply g by q^v.  */
        seq_t part = 1;
        for (unsigned int k = 0; k < v; ++k)
            part *= (seq_t)q;
        g *= part;
    }

    /* Final result: sigma / g (exact division). */
    return sigma / g;
}
