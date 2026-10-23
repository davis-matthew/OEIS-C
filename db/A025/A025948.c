/*
 * A025948  Expansion of 1/((1-2*x)*(1-3*x)*(1-8*x)*(1-9*x)).
 * https://oeis.org/A025948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1)-163*a(n-2)+462*a(n-3)-432*a(n-4) for n>=4
 *            Initial values: a(0)=1, a(1)=22, a(2)=321, a(3)=3938
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* number of stored elements */

    /* initialise cache with first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
        cache[2] = (seq_t)321;
        cache[3] = (seq_t)3938;
    }

    /* if requested index already cached, return it */
    if ((size_t)n < cache_size)
        return cache[(size_t)n];

    /* extend the cache up to index n */
    size_t new_size = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));

    /* compute missing terms incrementally */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)22 * cache[i - 1]
                 - (seq_t)163 * cache[i - 2]
                 + (seq_t)462 * cache[i - 3]
                 - (seq_t)432 * cache[i - 4];
    }

    cache_size = new_size;
    return cache[(size_t)n];
}
