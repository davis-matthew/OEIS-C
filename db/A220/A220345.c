/*
 * A220345  Ratio of Jordan's totient to Carmichael's lambda function: a(n) =
 *          A007434(n) / A002322(n).
 * https://oeis.org/A220345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = J_2(n) / λ(n) where J_2(n) = n^2∏_{p|n}(1‑1/p^2) (Jordan
 *            totient of order 2) and λ(n) is Carmichael’s λ-function
 * Range:     u8 n <= 16, u16 n <= 625, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_cache, factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t, uint64_t *, uint64_t *, size_t *); */
#include "../utils/ensure_cache.h"/* void ensure_cache(idx_t, seq_t **, size_t *); */
#include "../utils/gcd.h"          /* seq_t gcd(seq_t, seq_t); */

static seq_t *cache = NULL;     /* 1‑based cache for computed values */
static size_t cache_cap = 0;    /* current capacity of the cache */

seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;          /* undefined for 0, safe guard */

    /* Make sure the cache can hold up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Return cached value if already computed */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Special case: n == 1 */
    if (n == 1U)
    {
        cache[1] = (seq_t)1;
        return cache[1];
    }

    /* Factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t factor_cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &factor_cnt);

    seq_t J2 = (seq_t)1;   /* Product of J_2 contributions */
    seq_t lam = (seq_t)1;  /* lcm of λ contributions */

    for (size_t i = 0; i < factor_cnt; ++i)
    {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* Compute J_2(p^e) = p^{2e-2} * (p^2 - 1) */
        seq_t powp2 = (seq_t)1;
        for (idx_t k = 0; k < (idx_t)(2 * e - 2); ++k)
            powp2 *= (seq_t)p;

        seq_t factor = (seq_t)(p * p) - (seq_t)1;   /* p^2 - 1 */
        seq_t termJ = powp2 * factor;
        J2 *= termJ;

        /* Compute λ(p^e) */
        seq_t termLam;
        if (p == 2U)
        {
            if (e == 1U)
                termLam = (seq_t)1;
            else if (e == 2U)
                termLam = (seq_t)2;
            else
            {
                seq_t pow2 = (seq_t)1;
                for (idx_t k = 0; k < (idx_t)(e - 2); ++k)
                    pow2 *= (seq_t)2;   /* 2^{e-2} */
                termLam = pow2;
            }
        }
        else
        {
            seq_t powp = (seq_t)1;
            for (idx_t k = 0; k < (idx_t)(e - 1); ++k)
                powp *= (seq_t)p;          /* p^{e-1} */
            termLam = powp * (seq_t)(p - 1);
        }

        /* Update λ as lcm(λ, termLam) */
        seq_t g = gcd(lam, termLam);
        lam = lam / g * termLam;           /* division first to avoid overflow */
    }

    /* Final result: a(n) = J2 / λ */
    seq_t result = J2 / lam;

    cache[n] = result;
    return result;
}
