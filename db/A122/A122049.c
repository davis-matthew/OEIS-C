/*
 * A122049  a(n) = a(n-1) - (n-4)*a(n-4), with a(0)=0, a(1)=1, a(2)=2, a(3)=1.
 * https://oeis.org/A122049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) - (n-4)*a(n-4) Initial values: a(0)=0, a(1)=1,
 *            a(2)=2, a(3)=1
 * Range:     i8 n <= 11, i16 n <= 19, i32 n <= 33, i64 n <= 55, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Handle the first four terms directly */
    if (n < 4) {
        return n == 0 ? (seq_t)0 :
               n == 1 ? (seq_t)1 :
               n == 2 ? (seq_t)2 : (seq_t)1;
    }

    /* Initialise cache to hold the first four values if required */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) return (seq_t)0;           /* fault‑tolerant fallback */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
    }

    /* Grow cache if the requested index is beyond the current size */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) return cache[cache_size-1]; /* fault‑tolerant fallback */
        cache = tmp;

        /* Compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i-1] - (seq_t)(i-4) * cache[i-4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
