/*
 * A125204  a(0)=0, a(1)=1; and a(n) = a(n-1) + a(a(n-1) mod n) for n>=2.
 * https://oeis.org/A125204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1; a(n)=a(n-1)+a(a(n-1) mod n) for n>=2
 * Range:     u8 n <= 17, u16 n <= 63, u32 n <= 178, u64 n <= 647,
 *            u128 n <= 2350, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* types seq_t and idx_t are provided by the build */
seq_t a(idx_t n)
{
    /* static cache to store already computed a-values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements stored in cache */

    /* initialise first two values */
    if (cache_len == 0) {
        cache     = (seq_t *)calloc(2, sizeof(seq_t));
        if (cache == NULL) abort();           /* memory exhausted */
        cache[0]  = (seq_t)0;
        cache[1]  = (seq_t)1;
        cache_len = 2;
    }

    /* if n≥current cache size, extend cache and compute the missing terms */
    if (n >= cache_len) {
        /* allocate memory for indices 0..n */
        seq_t *new_cache = (seq_t *)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (new_cache == NULL) abort();
        cache = new_cache;

        /* compute all a[i] for i from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < 2) continue;              /* a(0) and a(1) already set */

            seq_t prev = cache[i - 1];
            /* compute a(prev mod i) */
            idx_t mod_index = (idx_t)(prev % (idx_t)i);
            cache[i] = prev + cache[mod_index];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
