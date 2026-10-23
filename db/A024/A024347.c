/*
 * A024347  Expansion of 1/((1-x)*(1-6*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A024347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 28*a(n-1) - 261*a(n-2) + 882*a(n-3) - 648*a(n-4), with
 *            a(0)=1, a(1)=28, a(2)=523, a(3)=8218
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms */
    static const seq_t init[4] = { (seq_t)1, (seq_t)28, (seq_t)523, (seq_t)8218 };
    if (n < 4)
        return init[n];

    /* Static cache for previously computed values */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of stored terms */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Initialize cache on first use */
    if (cache == NULL) {
        cache_cap  = 8;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = init[0];
        cache[1] = init[1];
        cache[2] = init[2];
        cache[3] = init[3];
        cache_size = 4;
    }

    /* Grow cache and compute missing terms up to n */
    while (cache_size <= (size_t)n) {
        /* Ensure capacity */
        if (cache_size == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* Compute next term using the recurrence */
        seq_t v1 = cache[cache_size - 1];
        seq_t v2 = cache[cache_size - 2];
        seq_t v3 = cache[cache_size - 3];
        seq_t v4 = cache[cache_size - 4];

        seq_t val = 28 * v1;
        val += 882 * v3;
        val -= 261 * v2;
        val -= 648 * v4;

        cache[cache_size++] = val;
    }

    return cache[(size_t)n];
}
