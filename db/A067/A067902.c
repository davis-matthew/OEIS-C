/*
 * A067902  a(n) = 14*a(n-1) - a(n-2); a(0) = 2, a(1) = 14.
 * https://oeis.org/A067902
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 14*a(n-1) - a(n-2), a(0) = 2, a(1) = 14
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;   /* pointer to cached values            */
    static idx_t cache_cap = 0;   /* allocated capacity (in elements)    */
    static idx_t cache_len = 0;   /* number of elements actually valid   */

    /* Initialise cache for the first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache_len = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out‑of‑memory fallback */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)14;
    }

    /* Return cached result if available */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache has enough capacity */
    while (cache_cap <= n) {
        idx_t new_cap = cache_cap > 0 ? cache_cap * 2 : 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* out‑of‑memory fallback */
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute new terms up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t term = cache[i-1] * (seq_t)14;  /* 14*a(i-1) */
        term -= cache[i-2];                  /* -a(i-2)   */
        cache[i] = term;
    }

    cache_len = n + 1;   /* all indices 0..n are now valid */
    return cache[n];
}
