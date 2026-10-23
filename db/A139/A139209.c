/*
 * A139209  Fibonacci bisection minus powers of 2.
 * https://oeis.org/A139209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = F(2n) – 2^n Recurrence used: a(0)=a(1)=a(2)=-1
 *            a(n)=5*a(n-1)-7*a(n-2)+2*a(n-3) (n≥3)
 * Range:     i8 n <= 6, i16 n <= 11, i32 n <= 23, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffers to cache already computed values */
    static seq_t *cache = NULL;   /* pointer to the array */
    static idx_t  cache_len  = 0; /* allocated length */
    static idx_t  cache_done = 0; /* number of values already computed */

    /* If we need a value that has already been computed, return it. */
    if (n < cache_done)
        return cache[n];

    /* first time we are called: initialise the cache with the first
       three terms of the sequence                                            */
    if (cache == NULL) {
        cache_len = (n + 1 > 3 ? n + 1 : 3);
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL)          /* allocation failed; give up            */
            return 0;
        cache[0] = -1;
        cache[1] = -1;
        cache[2] = -1;
        cache_done = 3;
    }

    /* if we later need more space, grow the cache */
    if (n + 1 > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL)
            return 0;
        cache = tmp;
        cache_len = n + 1;
    }

    /* compute any missing values up to n using the recurrence */
    for (idx_t k = cache_done; k <= n; ++k) {
        if (k < 3)           /* base cases already known                */
            continue;
        cache[k] = 5 * cache[k - 1] - 7 * cache[k - 2] + 2 * cache[k - 3];
    }
    cache_done = n + 1;          /* we now have values up to index n */
    return cache[n];
}
