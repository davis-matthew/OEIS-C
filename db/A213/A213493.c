/*
 * A213493  Number of (w,x,y) with all terms in {0,...,n} and the numbers
 *          w,x,y,|w-x|,|x-y|,|y-w| distinct.
 * https://oeis.org/A213493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - 2*a(n-5) + a(n-8) + a(n-9) - a(n-10),
 *            with a(0..10) = {0,0,0,0,0,0,12,48,96,204,300} (offset 0)
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
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

/* Expected types provided by the build system: seq_t, idx_t. */
seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cap = 0;      /* allocated capacity of cache */
    static idx_t computed = 0; /* highest index already computed */
    static bool init_done = false;

    /* Initial known values up to index 10 */
    if (!init_done) {
        /* ensure at least 11 entries (0..10) */
        ensure_cache(10, &cache, &cap);
        seq_t init_vals[11] = {0,0,0,0,0,0,12,48,96,204,300};
        for (idx_t i = 0; i <= 10; ++i) cache[i] = init_vals[i];
        computed = 10;
        init_done = true;
    }

    /* Ensure cache is large enough for requested index */
    if (n >= cap) /* require capacity for indices 0..n inclusive */
        ensure_cache(n, &cache, &cap);

    /* If already computed, just return */
    if (n <= computed)
        return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Use recurrence:
           a(i) = a(i-1) + a(i-2) - 2*a(i-5) + a(i-8) + a(i-9) - a(i-10) */
        seq_t val = cache[i - 1] + cache[i - 2];
        val -= (seq_t)2 * cache[i - 5];
        val += cache[i - 8] + cache[i - 9];
        val -= cache[i - 10];
        cache[i] = val;
    }
    computed = n;
    return cache[n];
}
