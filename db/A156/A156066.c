/*
 * A156066  Numbers k such that k^2 is a square arising in A154138.
 * https://oeis.org/A156066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 50, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A156066: a(n) = 6*a(n-2) - a(n-4) for n>4,
 * a(1)=2, a(2)=3, a(3)=9, a(4)=16.  */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached sequence values (1‑based index). */
    static seq_t *cache = NULL;      /* array of terms */
    static idx_t cache_len = 0;      /* number of terms stored */
    static idx_t cache_cap = 0;      /* allocated capacity */

    /* Protect against n=0 (outside OEIS offset). */
    if (n == 0)
        return (seq_t)0;

    /* Initialise the cache with the first four terms on the first call. */
    if (cache_len == 0) {
        idx_t init_len = 4;
        cache = (seq_t *)realloc(NULL, init_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;            /* allocation failure: abort */
        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)3;   /* a(2) */
        cache[2] = (seq_t)9;   /* a(3) */
        cache[3] = (seq_t)16;  /* a(4) */
        cache_len = init_len;
        cache_cap = init_len;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache up to the requested index. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* Grow the allocated array if necessary. */
        if (i > cache_cap) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap < i)
                new_cap = i;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return (seq_t)0;          /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Recurrence: a(i) = 6*a(i-2) - a(i-4). */
        seq_t val = (seq_t)6 * cache[i - 3] - cache[i - 5];
        cache[i - 1] = val;
    }

    cache_len = n;
    return cache[n - 1];
}
