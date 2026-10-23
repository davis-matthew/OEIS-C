/*
 * A042350  Numerators of continued fraction convergents to sqrt(702).
 * https://oeis.org/A042350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 106*a(n-2) - a(n-4) Initial values: a(0) = 26 a(1) = 53
 *            a(2) = 2782 a(3) = 5617
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache the already-computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;             /* allocation failed */
        cache[0] = 26;
        cache[1] = 53;
        cache[2] = 2782;
        cache[3] = 5617;
    }

    /* retrieve a previously computed term quickly */
    if (n < cache_size) return cache[n];

    /* grow the cache to accommodate index n */
    idx_t old_size = cache_size;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                     /* reallocation failed */
    cache = tmp;
    cache_size = new_size;

    /* compute all missing terms up to n using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = (seq_t)106 * cache[i - 2] - cache[i - 4];
    }

    return cache[n];
}
