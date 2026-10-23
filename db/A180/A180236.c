/*
 * A180236  a(n) = a(n-2)+a(n-4); a(1)=a(4)=101, a(2)=a(3)=10.
 * https://oeis.org/A180236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+a(n-4); a(1)=101, a(2)=10, a(3)=10, a(4)=101
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 78, u64 n <= 170, u128 n <= 355,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0; /* allocated size */
    static size_t cache_size = 0; /* number of initialized elements (max index+1) */

    /* Handle n < 1 – not expected, but return 0 for safety */
    if (n < 1u)
        return 0;

    /* Ensure the cache covers at least indices 0..4 with initial values. */
    if (cache == NULL) {
        cache_cap = 5;      /* indices 0..4 inclusive */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0; /* allocation failed */
        cache[0] = 0;      /* unused, but keeps array 1‑based */
        cache[1] = 101;
        cache[2] = 10;
        cache[3] = 10;
        cache[4] = 101;
        cache_size = 5;     /* maximum known index + 1 */
    }

    /* If n is within already computed range, just return it */
    if ((size_t)n < cache_size)
        return cache[n];

    /* Grow cache if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;   /* realloc failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_size; i <= (size_t)n; ++i) {
        /* recurrence a(i) = a(i-2) + a(i-4) */
        /* For i <= 4, cache already initialized */
        if (i > 4u)
            cache[i] = cache[i-2] + cache[i-4];
    }

    /* Update known size */
    cache_size = (size_t)n + 1;

    return cache[n];
}
