/*
 * A232509  Number of (n+1)X(2+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally,
 *          diagonally or antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-2) - 4*a(n-3) - 5*a(n-4) - 2*a(n-5) +
 *            a(n-6) + 2*a(n-7) with a(1)=16, a(2)=52, a(3)=200, a(4)=784,
 *            a(5)=3052, a(6)=11900, a(7)=46432
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms; element i holds a(i+1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;          /* number of cached terms */

    /* Initialise cache with the first seven values on first call */
    if (cache_len == 0) {
        cache_len = 7;
        cache = (seq_t *)malloc((size_t)cache_len * sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */
        cache[0] = (seq_t)16;      /* a(1) */
        cache[1] = (seq_t)52;      /* a(2) */
        cache[2] = (seq_t)200;     /* a(3) */
        cache[3] = (seq_t)784;     /* a(4) */
        cache[4] = (seq_t)3052;    /* a(5) */
        cache[5] = (seq_t)11900;   /* a(6) */
        cache[6] = (seq_t)46432;   /* a(7) */
    }

    /* If the requested term is already cached, return it */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow the cache to accommodate index n */
    idx_t old_len = cache_len;
    cache_len = n; /* new required size */
    seq_t *tmp = (seq_t *)realloc(cache, (size_t)cache_len * sizeof(seq_t));
    if (!tmp) {
        /* reallocation failed – abort */
        abort();
    }
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t k = old_len; k < n; ++k) { /* k indexes a(k+1) */
        cache[k] =
            cache[k - 1] * (seq_t)4
            + cache[k - 2]
            - cache[k - 3] * (seq_t)4
            - cache[k - 4] * (seq_t)5
            - cache[k - 5] * (seq_t)2
            + cache[k - 6]
            + cache[k - 7] * (seq_t)2;
    }

    return cache[n - 1];
}
