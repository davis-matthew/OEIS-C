/*
 * A197617  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,3,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + a(n-4) (n ≥ 5) Initial values:
 *            a(1)=0, a(2)=0, a(3)=8, a(4)=0 The function keeps a static cache
 *            of computed terms
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 144,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the small indices that are needed for the recurrence. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)8;
    if (n == 4) return (seq_t)0;

    /* Static cache for previously computed values. */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* largest index that is currently valid */

    /* Allocate or grow the cache if we need to store up to index n. */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;            /* +1 because we index from 1 */
        if (new_cap < 64) new_cap = 64;           /* avoid very small blocks */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                       /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        /* Initialize the cache on first allocation. */
        if (cache_len == 0) {
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)8;
            cache[4] = (seq_t)0;
            cache_len = 4;
        }
    }

    /* Compute any missing terms up to the requested index n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t val = cache[i - 2]
                  + (seq_t)4 * cache[i - 3]
                  + cache[i - 4];
        cache[i] = val;
    }
    cache_len = (size_t)n;

    return cache[(size_t)n];
}
