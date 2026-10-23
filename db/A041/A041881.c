/*
 * A041881  Denominators of continued fraction convergents to sqrt(462).
 * https://oeis.org/A041881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 86*a(n-2) - a(n-4) for n > 3 * Initial values: * a(0) = 1
 *            * a(1) = 2 * a(2) = 85 * a(3) = 172
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;      /* pointer to the allocated array */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of computed elements */

    /* Initialize cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* allocation failure – return 0 as safe value */

        /* base terms */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)85;
        cache[3] = (seq_t)172;
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if (n < cache_len) {
        return cache[(size_t)n];
    }

    /* Grow cache and compute up to n */
    while (cache_len <= (size_t)n) {
        /* Ensure capacity */
        if ((size_t)cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;  /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term via recurrence */
        seq_t val = ((seq_t)86 * cache[(size_t)(cache_len - 2)]) - cache[(size_t)(cache_len - 4)];
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[(size_t)n];
}
