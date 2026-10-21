/*
 * A077416  Chebyshev S-sequence with Diophantine property.
 * https://oeis.org/A077416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - a(n-2), a(0) = 1, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed_upto = 0;  /* largest index that has a value */

    /* Initialise cache on the first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)13;
        computed_upto = 1;
    }

    /* Grow cache if required */
    if (n > computed_upto) {
        /* Allocate size at least n+1, preferably doubled to reduce reallocs */
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = needed * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms via the recurrence */
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)12 - cache[i - 2];
        }
        computed_upto = n;
    }

    return cache[n];
}
