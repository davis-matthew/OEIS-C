/*
 * A166117  a(0)=0, a(1)=1, a(2)=2 and a(n) = a(n-1) - 2a(n-2) + a(n-3).
 * https://oeis.org/A166117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=2, a(n)=a(n-1)-2*a(n-2)+a(n-3) for n≥3
 * Range:     i8 n <= 19, i16 n <= 37, i32 n <= 77, i64 n <= 155,
 *            i128 n <= 313, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* small indices handled directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* static cache */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* initialise cache if first call */
    if (cache == NULL) {
        /* start with capacity at least enough for n+1 but not too small */
        cache_cap = (size_t)(n + 1) < 4 ? 4 : (size_t)(n + 1);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – return 0 as safe value */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache_size = 3;
    }

    /* grow cache if needed */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed – return 0 as safe value */
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] - ( (seq_t)2 * cache[i-2] ) + cache[i-3];
    }
    cache_size = n + 1;

    return cache[n];
}
