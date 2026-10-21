/*
 * A052543  Expansion of (1-x)/(1 - 3*x - 2*x^2 + 2*x^3).
 * https://oeis.org/A052543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) + 2*a(n-2) - 2*a(n-3) with a(0)=1, a(1)=2,
 *            a(2)=8 (OEIS A052543)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoised values */
    static idx_t cache_cap = 0;          /* allocated capacity (elements) */
    static idx_t cache_size = 0;         /* number of values already stored */

    /* Ensure the cache has space for at least the first 3 entries */
    if (cache_size == 0) {
        cache_cap = 4;                   /* at least 4 to hold 0..3 safely */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        /* initialise with the known base values */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)8;
        cache_size = 3;
    }

    /* Grow the cache if we need to compute beyond the current capacity */
    if ((idx_t)(n + 1) > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 4;
        while (new_cap <= (idx_t)(n + 1))
            new_cap <<= 1;               /* double until it is large enough */

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* allocation failed – terminate with undefined value */
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to n (inclusive) */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        if (i == 0)
            val = (seq_t)1;
        else if (i == 1)
            val = (seq_t)2;
        else if (i == 2)
            val = (seq_t)8;
        else
            val = (seq_t)3 * cache[i - 1]
                + (seq_t)2 * cache[i - 2]
                - (seq_t)2 * cache[i - 3];
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
