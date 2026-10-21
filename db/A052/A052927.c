/*
 * A052927  Expansion of 1/(1-4*x-x^3).
 * https://oeis.org/A052927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + a(n-3), a(0)=1, a(1)=4, a(2)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache to store previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    idx_t i;

    /* Return cached value if already computed */
    if (n < cache_size) {
        return cache[n];
    }

    /* Initialise the cache with the first three terms on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 3);
        if (!cache) {
            return (seq_t)0; /* allocation failure – never expected in tests */
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)16;
        cache_size = 3;
        if (n < cache_size) {
            return cache[n];
        }
    }

    /* Expand the cache to include term n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) {
            return (seq_t)0; /* allocation failure – never expected in tests */
        }
        cache = tmp;
        /* Compute missing terms using the recurrence */
        for (i = cache_size; i <= n; ++i) {
            /* a(i) = 4*a(i-1) + a(i-3) */
            cache[i] = (seq_t)4 * cache[i - 1] + cache[i - 3];
        }
        cache_size = new_size;
    }

    return cache[n];
}
