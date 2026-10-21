/*
 * A057021  Denominator of (sum of divisors of n / number of divisors of n).
 * https://oeis.org/A057021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = τ(n)/gcd(σ(n), τ(n)) where τ is the divisor‑count and σ
 *            the sum of divisors
 * Range:     u8 n <= 18, u16 n <= 2237, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility functions: factor_u64 (for prime factorisation), gcd (greatest common divisor),
   sigma_of (sum of divisors) */
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

/* Compute a(idx_t n) where n ≥ 1.  The function is fully deterministic and
   independent of previous calls. It works for any build of seq_t/idx_t. */
seq_t a(idx_t n)
{
    /* Factorise n once, obtaining primes and exponents. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute τ(n) = product (e_i+1). */
    seq_t tau = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t ee = (seq_t)(exps[i] + 1);
        tau *= ee;
    }

    /* Compute σ(n) = product σ(p_i^e_i) using the multiplicative property. */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* Compute p^(e+1). */
        seq_t power = (seq_t)1;
        for (uint64_t j = 0; j <= e; ++j)
            power *= (seq_t)p;    /* now power = p^(e+1) */

        /* σ(p^e) = (p^(e+1)-1)/(p-1). */
        seq_t term = (power - (seq_t)1) / (seq_t)(p - 1);
        sigma *= term;
    }

    /* Reduce the fraction σ/τ and return the denominator τ/gcd(σ,τ). */
    seq_t g = gcd(sigma, tau);
    return tau / g;
}
