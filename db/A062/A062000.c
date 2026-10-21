/*
 * A062000  a(n) = a(n-1)^2 - a(n-2)^2 with a(0) = 0, a(1) = 2.
 * https://oeis.org/A062000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)^2 - a(n-2)^2 where a(0) = 0, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

/* Static cache for computed values */
static seq_t *cache = NULL;      /* array of computed terms */
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */

/* Ensure that cache has capacity for at least 'need' elements */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
        while (new_cap < need)
            new_cap *= 2;
        seq_t *new_cache = alloc_seq_array(new_cap);
        if (cache) {
            /* copy old values */
            for (size_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
            free_seq_array(cache);
        }
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise cache with first two terms */
    if (cache == NULL) {
        grow_cache(2);
        cache[0] = 0;
        cache[1] = 2;
        cache_len = 2;
    }

    /* Return if already computed */
    if ((size_t)n < cache_len)
        return cache[n];

    /* Grow cache if needed */
    grow_cache((size_t)n + 1);

    /* Compute missing terms iteratively */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t x1 = cache[i - 1];
        seq_t x2 = cache[i - 2];
        cache[i] = x1 * x1 - x2 * x2;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
