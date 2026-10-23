/*
 * A179237  a(0) = 1, a(1) = 2; a(n+1) = 6*a(n) + a(n-1) for n>1.
 * https://oeis.org/A179237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Computes the n-th term of OEIS A179237 */
seq_t a(idx_t n)
{
    /* Handle the two initial cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Initialise on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failed – unlikely in test */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_size = 2;
    }

    /* If we already have the value, return it */
    if ((size_t)n < cache_size) return cache[(size_t)n];

    /* Extend the cache up to n */
    size_t new_size = (size_t)n + 1;
    if (new_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0; /* reallocation failed – unlikely */
        cache = tmp;
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)6 * cache[i - 1] + cache[i - 2];
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
