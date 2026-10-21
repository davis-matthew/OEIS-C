/*
 * A239285  a(n) = (15^n - (-2)^n)/17.
 * https://oeis.org/A239285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 0, a(1) = 1
 * Recurrence: a(n) = 13*a(n-1) + 30*a(n-2)   for n >= 2
 * Equivalent closed form: a(n) = (15^n - (-2)^n)/17
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;    /* dynamic array holding computed terms   */
static size_t cache_cap = 0;   /* current capacity of cache              */
static idx_t last = 0;         /* last index that has been computed      */

seq_t a(idx_t n)
{
    /* First time: allocate space for at least the initial two terms */
    if (cache == NULL) {
        idx_t init_cap = (n < 2) ? 2 : n + 1;
        ensure_cache(init_cap, &cache, &cache_cap);
        cache[0] = (seq_t)0;
        if (init_cap > 1)
            cache[1] = (seq_t)1;
        last = 1;           /* we've computed up to index 1       */
    }

    /* Extend cache and compute missing terms on demand              */
    if ((idx_t)last < n) {
        /* Ensure there is enough space to hold the new terms */
        if ((idx_t)cache_cap <= n) {
            idx_t new_cap = n + 1;
            ensure_cache(new_cap, &cache, &cache_cap);
        }

        /* Compute from last+1 up to n using the recurrence          */
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)13 + cache[i - 2] * (seq_t)30;
        }
        last = n;          /* update the highest computed index      */
    }

    return cache[(idx_t)n];
}
