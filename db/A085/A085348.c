/*
 * A085348  Ratio-determined insertion sequence I(0.264) (see the link below).
 * https://oeis.org/A085348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=19, a(3)=72; a(n) = 18*a(n-2) - a(n-4) for
 *            n >= 4 (sequential recurrence with order 4)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoised values */
    static seq_t *cache = NULL;          /* pointer to cached terms */
    static idx_t cache_cap   = 0;        /* capacity of cache array   */
    static idx_t cache_size  = 0;        /* number of terms already filled */

    /* Allocate initial capacity if this is the first call */
    if (cache == NULL) {
        cache_cap = 16;   /* arbitrary initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Grow cache if necessary */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= n) new_cap <<= 1;      /* double until large enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* If we have already computed up to n, just return it */
    if (n < cache_size)
        return cache[n];

    /* Compute terms from the current cache size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0)          cache[i] = (seq_t)1;
        else if (i == 1)     cache[i] = (seq_t)4;
        else if (i == 2)     cache[i] = (seq_t)19;
        else if (i == 3)     cache[i] = (seq_t)72;
        else {
            /* Recurrence: a(i) = 18*a(i-2) - a(i-4) */
            cache[i] = (seq_t)18 * cache[i-2] - cache[i-4];
        }
    }

    cache_size = n + 1;  /* we now have all terms up to n */
    return cache[n];
}
