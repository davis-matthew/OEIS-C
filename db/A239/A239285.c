/*
 * A239285  a(n) = (15^n - (-2)^n)/17.
 * https://oeis.org/A239285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 799 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(0) = 0, a(1) = 1
 * Recurrence: a(n) = 13*a(n-1) + 30*a(n-2)   for n >= 2
 * Equivalent closed form: a(n) = (15^n - (-2)^n)/17
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

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
