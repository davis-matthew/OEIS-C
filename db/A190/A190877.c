/*
 * A190877  Expansion of e.g.f. exp(x+x^5).
 * https://oeis.org/A190877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 5! * C(n-1,4) * a(n-5) for n > 4 Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 36,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *cache = NULL;           /* array of computed terms            */
static size_t cache_len = 0;          /* number of elements currently stored */
static size_t cache_cap = 0;          /* current allocated capacity          */

/* Ensure the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Grow the cache if necessary. */
    if (n >= cache_cap) {
        size_t newcap = cache_cap ? cache_cap << 1 : 16;
        while (newcap <= (size_t)n)
            newcap <<= 1;
        seq_t *newcache = (seq_t *)malloc(newcap * sizeof(seq_t));
        if (!newcache) exit(1);               /* abort on allocation failure */

        if (cache) {
            for (size_t i = 0; i < cache_len; ++i)
                newcache[i] = cache[i];
            free(cache);
        }
        cache     = newcache;
        cache_cap = newcap;
    }

    /* Compute missing terms. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i <= 4) {
            cache[i] = (seq_t)1;
        } else {
            /* binomial(i-1,4) fits in 64 bits for n ≤ 10000, so we
               can safely compute it via the utility function. */
            seq_t bin = binomial(i - 1, (idx_t)4);
            seq_t mult = (seq_t)120 * bin;          /* 120 * C(i-1,4) */
            seq_t add  = cache[i - 5] * mult;       /* *a(i-5)   */
            cache[i]   = cache[i - 1] + add;        /* a(i-1)+add */
        }
    }
    cache_len = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
