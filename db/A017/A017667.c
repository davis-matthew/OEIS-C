/*
 * A017667  Numerator of sum of -2nd powers of divisors of n.
 * https://oeis.org/A017667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator of Σ_{d|n} 1/d², i.e. numerator of σ₂(n)/n²
 * Range:     u8 n <= 15, u16 n <= 241, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* factor_u64(n, primes, exponents, &count) is provided by the utils package */
#include "../utils/factor_u64.h"

/* A small gcd routine for uint64_t values.  It is independent of the
   seq_t type so that it works for every build. */
static uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b != 0)
    {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but return 0 safely */
        return (seq_t)0;

    /* Factor n into primes and exponents.  The arrays are large enough
       for any n up to the limits of the problem (n ≤ 10 000). */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute σ₂(n) = ∏ (p^{2(e+1)}-1)/(p²-1). */
    uint64_t sigma2 = 1ULL;

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* term = (p^{2(e+1)} - 1) / (p² - 1) .  All intermediate
           values comfortably fit in 64‑bit for n ≤ 10 000. */
        uint64_t num = 1;
        for (uint64_t j = 0; j < 2 * (e + 1); ++j)
            num *= p;
        num -= 1;
        uint64_t den = p * p - 1;
        uint64_t term = num / den;        /* exact division */

        sigma2 = (uint64_t)((__uint128_t)sigma2 * term);
    }

    /* n² and the greatest common divisor.  These fit in 64‑bit. */
    uint64_t n2 = (uint64_t)n * (uint64_t)n;
    uint64_t g  = gcd_uint64(sigma2, n2);

    /* Numerator after reducing the fraction σ₂(n)/n². */
    uint64_t numerator = sigma2 / g;

    return (seq_t)numerator;
}
