/*
 * A019664  Expansion of 1/((1-4x)(1-8x)(1-9x)).
 * https://oeis.org/A019664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=21, a(2)=301, a(n)=21*a(n-1)-140*a(n-2)+288*a(n-3)
 *            for n>2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored (next free index) */
    static size_t cache_cap  = 0;   /* capacity of array */

    /* ensure capacity for at least three terms */
    if (cache_cap < 3) {
        cache_cap = (size_t)(n > 2 ? n + 1 : 3);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)301;
        cache_size = 3;
    }

    /* If the requested index is already in the cache, return it */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache if necessary */
    if (n + 1 > cache_cap) {
        cache_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        seq_t val = (seq_t)21 * cache[i - 1]
                  - (seq_t)140 * cache[i - 2]
                  + (seq_t)288 * cache[i - 3];
        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
    return cache[n];
}
