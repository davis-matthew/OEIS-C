/*
 * A122099  a(n) = -3*a(n-1) + a(n-3) for n>2, with a(0)=1, a(1)=1, a(2)=0.
 * https://oeis.org/A122099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   a(n) = -3*a(n-1) + a(n-3) Initial values: a(0) = 1, a(1) = 1,
 *            a(2) = 0
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 23, i64 n <= 44, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static idx_t  last_done  = 2;          /* last index fully computed */

    /* Handle the base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    /* First call: initialise a 3-element cache */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        last_done = 2;
    }

    /* Grow the cache if needed */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute any missing terms up to n */
    for (idx_t k = last_done + 1; k <= n; ++k) {
        cache[k] = (seq_t)(- (seq_t)3 * cache[k - 1] + cache[k - 3]);
    }
    last_done = n;

    return cache[n];
}
