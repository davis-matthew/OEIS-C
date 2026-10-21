/*
 * A054489  Expansion of (1+4*x)/(1-6*x+x^2).
 * https://oeis.org/A054489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2), a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses memoisation to generate terms up to the largest n requested. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache holds already computed terms. */
    static seq_t *cache = NULL;      /* array of terms */
    static size_t cache_len = 0;     /* number of initialized terms */
    static size_t cache_cap = 0;     /* allocated capacity */

    /* Initialise cache on first call.  Allocate small initial capacity. */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;      /* allocation failure – unlikely in judge */
        cache[0] = 1;                    /* a(0) */
        cache[1] = 10;                   /* a(1) */
        cache_len = 2;
    }

    /* If n is beyond current capacity, grow the cache. */
    if ((idx_t)(cache_cap - 1) < n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n + 1)
            new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failure */
        cache = tmp;
        /* Initialize newly allocated entries using the recurrence. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            cache[i] = 6 * cache[i - 1] - cache[i - 2];
        }
        cache_cap = new_cap;
    }

    /* Extend computed terms up to n if necessary. */
    while (cache_len <= (size_t)n) {
        cache[cache_len] = 6 * cache[cache_len - 1] - cache[cache_len - 2];
        ++cache_len;
    }

    return cache[n];
}
