/*
 * A026243  a(n) = A000522(n) - 2.
 * https://oeis.org/A026243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A026243: a(n) = n*(a(n-1)+2)-1   (for n>1),  a(1)=0
   Offset 1:  a(1) is the first term. */

/* Required standard headers */
#include <stdlib.h>   /* malloc, realloc */

/* Return the nth term of A026243 (offset 1). */
seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1, just return 0 */
        return (seq_t)0;

    /* Cache: cache[k-1] stores a(k)  (k >= 1) */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;     /* highest n stored in cache */
    static size_t cache_cap = 0;   /* allocated capacity of cache  (>= cached_n) */

    /* Initialise cache with a(1) if it does not yet exist */
    if (cached_n == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)                /* allocation failure – give up */
            return (seq_t)0;
        cache[0] = (seq_t)0;       /* a(1) = 0 */
        cached_n = 1;
        cache_cap = 1;
    }

    /* Extend cache up to the requested n */
    if (n > cached_n) {
        size_t new_cap = (size_t)n;            /* new capacity (elements) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                            /* realloc failed – keep old cache */
            return cache[cached_n - 1];
        }
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms using the recurrence */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            /* cache[i-2] holds a(i-1) */
            seq_t prev = cache[i - 2];          /* a(i-1) */
            cache[i - 1] = (seq_t)i * (prev + (seq_t)2) - (seq_t)1;
        }
        cached_n = n;
    }

    /* Return the requested value */
    return cache[n - 1];
}
