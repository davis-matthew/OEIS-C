/*
 * A018905  Duplicate of A024537.
 * https://oeis.org/A018905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 1 (with a(0)=2, a(1)=4)
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 14, i64 n <= 14, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_up_to = (idx_t)-1;  /* no values computed yet */

    /* Initialize cache with the first two terms */
    if (cache == NULL) {
        cache_cap = 4;                     /* start with small capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) abort();        /* memory error */

        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cached_up_to = 1;
    }

    /* Return the value if already computed */
    if (n <= cached_up_to) {
        return cache[(size_t)n];
    }

    /* Ensure capacity for up to n+1 elements */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)(n + 1) > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();          /* memory error */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[(size_t)(i - 1)] + cache[(size_t)(i - 2)] - (seq_t)1;
        cache[(size_t)i] = val;
    }
    cached_up_to = n;
    return cache[(size_t)n];
}
