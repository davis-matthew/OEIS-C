/*
 * A045626  Bends in loxodromic sequence of spheres in which each 5
 *          consecutive spheres are in mutual contact.
 * https://oeis.org/A045626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)-a(n-5), with a(0) = -1, a(1)
 *            = 2, a(2) = 3, a(3) = 3, a(4) = 5
 * Range:     i8 n <= 8, i16 n <= 17, i32 n <= 34, i64 n <= 70, i128 n <= 140,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached values for memoisation */
static seq_t *cache = NULL;
static size_t cache_size = 0;

/* Return the n-th term of OEIS A045626 */
seq_t a(idx_t n)
{
    /* Initialise the first five terms on the first call */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)-1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)5;
    }

    /* Return if the value is already cached */
    if (n < (idx_t)cache_size)
        return cache[n];

    /* Grow the cache to hold the requested index */
    size_t new_len = (size_t)(n + 1);          /* store indices [0..n] */
    seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) return (seq_t)0;                /* allocation failure */
    cache = tmp;

    /* Compute any new terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i-1] + cache[i-2] + cache[i-3] + cache[i-4]
                   - cache[i-5];
    }
    cache_size = new_len;
    return cache[n];
}
