/*
 * A098524  Expansion of (1+2x^2)/(1-x-4x^5).
 * https://oeis.org/A098524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n‑1) + 4*a(n‑5); a(0)=1, a(1)=1, a(2)=3, a(3)=3, a(4)=3
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the sequence A098524 with memoisation.  */

#include <stdlib.h>

/* Static cache for computed terms.  */
static seq_t *cache = NULL;          /* (seq_t) array of cached values      */
static idx_t cache_size = 0;         /* number of terms already stored      */
static idx_t cache_cap  = 0;         /* allocated capacity (number of entries)  */

/* Ensure that cache can hold indices 0 … n.  */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > n)
        return;                      /* already large enough. */
    idx_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap <= n)
        new_cap <<= 1;                /* grow by powers of two */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) {
        /* In a contest environment, aborting is acceptable. */
        abort();
    }
    cache     = new_cache;
    cache_cap = new_cap;
}

/* Sequence generator: a(n).  */
seq_t a(idx_t n)
{
    /* Provide the first five values directly.  */
    static const seq_t init[5] = { 1, 1, 3, 3, 3 };

    /* Small indices can be answered immediately.  */
    if (n < 5)
        return init[n];

    /* Initialise the cache with the first five values if not yet done.  */
    if (cache_size < 5) {
        ensure_capacity(4);           /* need space up to index 4 */
        for (idx_t i = 0; i < 5; ++i)
            cache[i] = init[i];
        cache_size = 5;
    }

    /* Compute terms up to n that are missing.  */
    if (n >= cache_size) {
        ensure_capacity(n);           /* make sure we have room */
        for (idx_t i = cache_size; i <= n; ++i) {
            /* cache[i-5] and cache[i-1] are guaranteed to be present.  */
            cache[i] = cache[i - 1] + (seq_t)4 * cache[i - 5];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
