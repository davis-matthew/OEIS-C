/*
 * A211323  Number of (n+1) X (n+1) -3..3 symmetric matrices with every 2 X 2
 *          subblock having sum zero and three distinct values.
 * https://oeis.org/A211323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 4*a(n-1) - 4*a(n-2) - a(n-3) + 2*a(n-4)
 * Initial values: a(1)=14, a(2)=24, a(3)=42, a(4)=76
 */

#include <stddef.h>   /* size_t */
#include <string.h>   /* memcpy */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache        = NULL;     /* array of seq_t, 1‑based indexing */
    static size_t cache_size   = 0;        /* allocated capacity (elements) */
    static idx_t next_computed = 5;        /* next index to compute (next_computed‑1 is last filled) */

    /* initialise cache with the first four values */
    if (cache == NULL) {
        cache_size = 5;                    /* indices 0..4 are in use */
        cache = seq_alloc_new_array(cache_size);

        /* cache[0] is unused but zero‑initialised for safety */
        cache[0] = 0;
        cache[1] = 14;
        cache[2] = 24;
        cache[3] = 42;
        cache[4] = 76;
    }

    /* enlarge the cache if needed for the requested index n */
    if ((size_t)n >= cache_size) {
        size_t new_cap = cache_size;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */

        seq_t *new_cache = seq_alloc_new_array(new_cap);
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        seq_free_array(cache);          /* free the old buffer */
        cache      = new_cache;
        cache_size = new_cap;
    }

    /* compute missing terms only when required */
    if (n >= next_computed) {
        for (idx_t i = next_computed; i <= n; ++i) {
            /* use only additions and a final subtraction to avoid underflows */
            seq_t sum1 = cache[i-1] * 4 + cache[i-4] * 2;
            seq_t sum2 = cache[i-2] * 4 + cache[i-3];
            cache[i]   = sum1 - sum2;   /* guaranteed non‑negative by the recurrence */
        }
        next_computed = n + 1;
    }

    return cache[n];
}
