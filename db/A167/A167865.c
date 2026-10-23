/*
 * A167865  Number of partitions of n into distinct parts greater than 1, with
 *          each part divisible by the next.
 * https://oeis.org/A167865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0) = 1; for n>=1, a(n) = Sum_{d|n, d>1} a((n-d)/d)
 * Range:     u8 n <= 2417, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* --- integer sqrt for idx_t -------------------------------------- */
static inline idx_t floor_sqrt_idx(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > n / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* --- memoisation cache ------------------------------------------- */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t computed_max = 0;

/* ensure cache has room for indices up to n; initialise new slots */
static void ensure_cache(idx_t n) {
    if (cache_cap > n)
        return;
    idx_t new_cap = (n + 1) * 2;           /* grow by factor 2 */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);
    if (cache) {
        for (idx_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
    }
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;
    if (cache)
        free(cache);
    cache = new_cache;
    cache_cap = new_cap;
    if (cache_cap > 0)
        cache[0] = 1;
    if (cache_cap > 1)
        cache[1] = 0;
}

/* --- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);

    if (computed_max < n) {
        for (idx_t i = computed_max + 1; i <= n; ++i) {
            if (i == 0) { cache[0] = 1; continue; }
            if (i == 1) { cache[1] = 0; continue; }

            seq_t sum = 0;
            if (i > 1)
                sum += cache[0];             /* divisor d = i contributes a[0] */

            idx_t sq = floor_sqrt_idx(i);
            for (idx_t d = 2; d <= sq; ++d) {
                if (i % d == 0) {
                    idx_t k1 = i / d - 1;
                    sum += cache[k1];

                    idx_t d2 = i / d;
                    if (d2 != d && d2 > 1) {
                        idx_t k2 = d - 1;     /* because n/d2 = d */
                        sum += cache[k2];
                    }
                }
            }
            cache[i] = sum;
        }
        computed_max = n;
    }
    return cache[n];
}
