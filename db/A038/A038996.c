/*
 * A038996  Number of sublattices of index n in generic 9-dimensional lattice.
 * https://oeis.org/A038996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} Prod_{k=1..8} (p^(e+k)-1)/(p^k-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 12, u64 n <= 215,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Multiplicative over n, with the same exponent range for all primes.) */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"      /* factor_u64(uint64_t n, uint64_t *primes,
                                       uint64_t *exps, size_t *cnt) */
#include "../utils/seq_gcd.h"        /* seq_t seq_gcd(seq_t a, seq_t b) */

/* Helper: multiply two seq_t numbers safely (no overflow in arbitrary-precision
 * builds; for fixed-width builds assume values fit). */
static inline seq_t mul_seq(seq_t a, seq_t b) { return a * b; }

/* Helper: compute p^exp for a small exponent (exp <= 64). */
static seq_t pow_small(seq_t base, uint64_t exp) {
    seq_t res = (seq_t)1;
    while (exp) {
        if (exp & 1ULL) res = mul_seq(res, base);
        base = mul_seq(base, base);
        exp >>= 1ULL;
    }
    return res;
}

seq_t a(idx_t n) {
    if (n <= 1U) return (seq_t)1;   /* a(1)=1, a(0) unused */

    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        uint64_t e = exps[i];

        /* compute product_{k=1..8} (p^(e+k)-1)/(p^k-1) for this prime */
        seq_t prime_factor = (seq_t)1;

        /* p^e */
        seq_t p_pow_e = pow_small(p, e);

        seq_t p_pow_k = (seq_t)1;   /* p^0 initially */
        for (uint64_t k = 1; k <= 8; ++k) {
            p_pow_k = mul_seq(p_pow_k, p);      /* p^k */

            /* numerator = p^(e+k) - 1 = p_pow_e * p_pow_k - 1 */
            seq_t numerator = mul_seq(p_pow_e, p_pow_k);
            numerator -= (seq_t)1;

            /* denominator = p^k - 1 */
            seq_t denominator = p_pow_k;
            denominator -= (seq_t)1;

            /* multiply prime_factor by numerator and divide by denominator
             * keeping the result integer and avoiding overflow by canceling
             * common factors with GCD. */
            seq_t g1 = seq_gcd(prime_factor, denominator);
            prime_factor /= g1;
            denominator /= g1;

            seq_t g2 = seq_gcd(numerator, denominator);
            numerator /= g2;
            denominator /= g2;

            /* now denominator should divide prime_factor * numerator exactly */
            prime_factor = mul_seq(prime_factor, numerator);
            prime_factor /= denominator;
        }

        /* combine with previous primes */
        result = mul_seq(result, prime_factor);
    }

    return result;
}
