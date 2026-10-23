/*
 * A140781  a(n) = 10*a(n-1) - a(n-2).
 * https://oeis.org/A140781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - a(n-2), a(0)=1, a(1)=2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of precomputed indices (cache[0..cache_size-1]) */

    /* fast return if already computed */
    if (n < cache_size)
        return cache[n];

    /* size needed for the requested term */
    idx_t needed = n + 1;

    /* first allocation */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)needed);
        if (!cache)
            exit(EXIT_FAILURE);

        /* initialise the two base cases */
        cache[0] = (seq_t)1;
        if (n >= 1)
            cache[1] = (seq_t)2;
        cache_size = needed;

        /* compute remaining terms (if any) */
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = (seq_t)10 * cache[i-1] - cache[i-2];

        return cache[n];
    }

    /* grow cache if necessary */
    if (needed > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)needed);
        if (!tmp)
            exit(EXIT_FAILURE);
        cache = tmp;
        /* keep the old cache_size as the number of already valid values */
    }

    /* ensure the first two values are defined before continuing */
    idx_t cur = cache_size;
    if (cur == 0) {
        cache[0] = (seq_t)1;
        cur = 1;
    }
    if (cur == 1) {
        cache[1] = (seq_t)2;
        cur = 2;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cur; i <= n; ++i)
        cache[i] = (seq_t)10 * cache[i-1] - cache[i-2];

    /* update size information */
    cache_size = needed;

    return cache[n];
}
