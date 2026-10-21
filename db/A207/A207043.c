/*
 * A207043  Number of (n+1) X 2 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly one clockwise edge increases.
 * https://oeis.org/A207043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 8*a(n-3) - 2*a(n-4) for n>5 Initial
 *            values: a(1) = 81 a(2) = 150 a(3) = 441 a(4) = 1416 a(5) = 4371
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache keeps computed values across calls */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* highest index currently computed */

    if (n == 0) return 0; /* undefined for the sequence, but avoid UB */

    /* Initialize cache on first call (or if n is larger than previously cached) */
    if (cache_size == 0) {
        /* We at least need space up to 5 (initial values) */
        size_t init = n > 5 ? n : 5;
        cache = (seq_t *)malloc((init + 1) * sizeof(seq_t));
        if (cache == NULL) abort();          /* failed allocation */

        /* Store initial terms */
        cache[1] = (seq_t)81;
        cache[2] = (seq_t)150;
        cache[3] = (seq_t)441;
        cache[4] = (seq_t)1416;
        cache[5] = (seq_t)4371;

        /* Compute any values up to 'init' using the recurrence */
        for (idx_t i = 6; i <= init; ++i) {
            cache[i] = 2 * cache[i - 1]            /* 2*a(i-1) */
                     + cache[i - 2]                /* + a(i-2) */
                     + 8 * cache[i - 3]            /* + 8*a(i-3) */
                     - 2 * cache[i - 4];           /* - 2*a(i-4) */
        }
        cache_size = init;
    }
    else if ((size_t)n > cache_size) {
        /* Need to grow the cache up to 'n' */
        size_t newsize = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (newsize + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();  /* failed allocation */
        cache = tmp;
        /* Compute new entries */
        for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                     + cache[i - 2]
                     + 8 * cache[i - 3]
                     - 2 * cache[i - 4];
        }
        cache_size = (size_t)n;
    }

    return cache[n];
}
