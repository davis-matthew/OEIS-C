/*
 * A027657  Expansion of (1+4*x)/(1-34*x+x^2).
 * https://oeis.org/A027657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 38, a(n) = 34*a(n-1) - a(n-2) for n > 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* next free index */
    static size_t cache_cap  = 0;   /* capacity of cache */

    /* grow cache if necessary */
    if (cache == NULL || cache_cap <= n) {
        size_t new_cap = (cache_cap > 0) ? cache_cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* Out of memory – abort as this cannot be handled normally */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base values if they are not yet present */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }
    if (cache_size == 1 && n >= 1) {
        cache[1] = (seq_t)38;
        cache_size = 2;
    }

    /* Compute new terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)34 * cache[i-1] - cache[i-2];
    }
    cache_size = n + 1;

    return cache[n];
}
