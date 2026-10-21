/*
 * A235549  Number of (n+1) X (1+1) 0..1 arrays with the sum of each 2 X 2
 *          subblock two extreme terms minus its two median terms
 *          lexicographically nondecreasing rowwise and columnwise.
 * https://oeis.org/A235549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 4*a(n-2) - 8*a(n-3) - 4*a(n-4) + 8*a(n-5);
 *            initial values: a(1)=16, a(2)=46, a(3)=120, a(4)=288, a(5)=660
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined offset, return 0 */

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms (a(1)..a(cache_size)) */
    static size_t cache_cap = 0;    /* allocated capacity */

    /* initial five values */
    static const seq_t init[5] = {
        (seq_t)16,
        (seq_t)46,
        (seq_t)120,
        (seq_t)288,
        (seq_t)660
    };

    size_t nn = (size_t)n;         /* convert idx_t to size_t for array indices */

    /* initialise cache on first call */
    if (cache_size == 0) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();        /* unrecoverable allocation failure */

        /* store the first five terms */
        for (int i = 0; i < 5; ++i) cache[i] = init[i];
        cache_size = 5;
    }

    /* return already computed value */
    if (nn <= cache_size) return cache[nn - 1];

    /* extend cache if necessary */
    if (nn > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 64;
        while (new_cap < nn) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (size_t k = cache_size + 1; k <= nn; ++k) {
        seq_t val;
        /* recurrence: a(k) = 2*a(k-1)+4*a(k-2)-8*a(k-3)-4*a(k-4)+8*a(k-5) */
        val = (seq_t)2 * cache[k - 2]      /* 2*a(k-1)   */
            + (seq_t)4 * cache[k - 3]      /* 4*a(k-2)   */
            - (seq_t)8 * cache[k - 4]      /* 8*a(k-3)   */
            - (seq_t)4 * cache[k - 5]      /* 4*a(k-4)   */
            + (seq_t)8 * cache[k - 6];     /* 8*a(k-5)   */
        cache[cache_size++] = val;
    }

    return cache[nn - 1];
}
