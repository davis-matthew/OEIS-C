/*
 * A230170  Number of n X 2 0..2 arrays x(i,j) with each element horizontally,
 *          vertically, diagonally or antidiagonally next to at least one
 *          element with value 2-x(i,j).
 * https://oeis.org/A230170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 2*a(n-2) + 67*a(n-3) + 62*a(n-4) + 160*a(n-5)
 *            + 40*a(n-6) + 16*a(n-7); initial values: a(1)=3, a(2)=27,
 *            a(3)=193, a(4)=1407, a(5)=10211, a(6)=73417, a(7)=530771
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* cache[0] stores a(1) */
    static size_t cache_size = 0; /* number of terms cached */

    if (n == 0)
        return 0; /* undefined for n=0, but guard anyway */

    /* ensure we have initial 7 values in the cache */
    if (cache_size == 0) {
        /* allocate space for 7 terms (indices 0..6) */
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure fallback */
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)27;
        cache[2] = (seq_t)193;
        cache[3] = (seq_t)1407;
        cache[4] = (seq_t)10211;
        cache[5] = (seq_t)73417;
        cache[6] = (seq_t)530771;
        cache_size = 7;
    }

    /* if requested n is already cached, return it */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* otherwise grow the cache up to n and compute missing terms */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
    if (!new_cache)
        return 0; /* allocation failure fallback */
    cache = new_cache;

    /* compute terms from cache_size up to n-1 (zero-based) */
    for (size_t i = cache_size; i < (size_t)n; ++i) {
        /* recurrence uses the previous 7 terms */
        seq_t val = 6 * cache[i - 1]
                  - 2 * cache[i - 2]
                  + 67 * cache[i - 3]
                  + 62 * cache[i - 4]
                  + 160 * cache[i - 5]
                  + 40 * cache[i - 6]
                  + 16 * cache[i - 7];
        cache[i] = val;
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
