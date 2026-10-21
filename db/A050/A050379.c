/*
 * A050379  Number of ordered factorizations of n into members of A050376.
 * https://oeis.org/A050379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of ordered factorizations of n into members of A050376
 * Range:     u8 n <= 575, u16 n <= 9462, u32 n <= 9462, u64 n <= 9462,
 *            u128 n <= 9462, big any n
 * Helpers:   ensure_cache, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1; a(n) = sum_{d | n, d = p^t, p prime, t is a power of two} a(n/d) */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/ensure_cache.h"
#include "../utils/factor_u64.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Recursive helper that returns a(n) and fills the cache. */
static seq_t compute_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    ensure_cache(n, &cache, &cache_cap);

    if (cache[n] != 0)
        return cache[n];

    /* Factor n using factor_u64. */
    const size_t MAX_FACTORS = 64;          /* plenty for n <= 10^4 */
    uint64_t *primes = (uint64_t *)malloc(MAX_FACTORS * sizeof(uint64_t));
    uint64_t *exps   = (uint64_t *)malloc(MAX_FACTORS * sizeof(uint64_t));
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 0;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* enumerate t = 1, 2, 4, ... <= e (powers of two exponents) */
        uint64_t t = 1;
        while (t <= e) {
            /* compute d = p^t as idx_t (safe because p^t <= n) */
            uint64_t d = 1;
            for (uint64_t k = 0; k < t; ++k)
                d *= p;

            idx_t rest = n / d;
            res += compute_a(rest);

            t <<= 1;               /* next power of two */
        }
    }

    free(primes);
    free(exps);

    cache[n] = res;
    return res;
}

seq_t a(idx_t n)
{
    return compute_a(n);
}
