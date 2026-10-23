/*
 * A048823  a(n) = value of n defined as follows: start with n and reach 2 by
 *          repeatedly either dividing by d where d <= the square root or by
 *          subtracting 1. The division steps are free, but subtracting 1
 *          costs 1 point. The "value" of n is the smallest cost to reach 2.
 * https://oeis.org/A048823
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ a(n-1)+1, a(n/d) | 2 <= d <= sqrt(n), d | n }
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External linkage: seq_t a(idx_t n) */

#include <stdlib.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* first index that is NOT computed yet */
static idx_t cache_cap  = 0;   /* allocated capacity */

/* Ensure that cache holds values up to (and including) index n */
static void
ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size)
        return;          /* already computed */

    /* Grow capacity if needed */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
        if (new_cap < n + 1)
            new_cap = n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute new entries */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 2) {                 /* values below offset are irrelevant */
            cache[i] = 0;
            continue;
        }

        /* Base cost by subtracting 1 */
        seq_t best = cache[i - 1] + (seq_t)1;

        /* Try all free division steps */
        for (idx_t d = 2; d * d <= i; ++d) {
            if (i % d == 0) {
                idx_t j = i / d;
                seq_t cand = cache[j];
                if (cand < best)
                    best = cand;
            }
        }
        cache[i] = best;
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;   /* behaviour for n < offset is irrelevant */

    /* Initialise cache on first call */
    if (!cache) {
        cache_cap  = 64;
        cache_size = 3;               /* indices 0,1,2 are defined immediately */
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = 0;   /* a(2)=0 is set explicitly */
    }

    ensure_cache(n);
    return cache[n];
}
