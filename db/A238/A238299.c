/*
 * A238299  Second convolution of A107841.
 * https://oeis.org/A238299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n+2)*(2*n-1)*a(n) = 4*(5*n^2-2)*a(n-1) - (n-2)*(2*n+1)*a(n-2)
 *            with initial values a(0)=1, a(1)=4. The function caches
 *            previously computed terms so that calls in any order are fast
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   _compute_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_compute_term.h"

/* Cache for previously computed terms.  The array holds a(0) through
   a(cache_len-1).  It is grown on demand. */
static seq_t *cache = NULL;
static idx_t  cache_len = 0;   /* number of values currently stored */
static size_t cache_cap = 0;   /* allocated capacity in elements */

/* Ensure the cache can store terms at indices up to 'need' (inclusive). */
static void _grow_cache(idx_t need)
{
    if ((size_t)(need + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while ((size_t)(need + 1) > new_cap)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && new_cap != 0) {
            free(cache);
            cache = NULL;
            cache_cap = 0;
            return;
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n < cache_len)
        return cache[n];

    /* On first use, initialise the cache with the two known terms. */
    if (cache_len == 0) {
        _grow_cache(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache_len = 2;
    }

    /* Ensure enough space for the requested index. */
    if (n >= cache_len)
        _grow_cache(n);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0 || i == 1) {
            /* already initialised */
            continue;
        }
        cache[i] = _compute_term(i, cache);
    }
    cache_len = n + 1;
    return cache[n];
}
