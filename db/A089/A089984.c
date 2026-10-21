/*
 * A089984  1, 1, 1, 1, ... a, b, c, d, ac-bd, ...
 * https://oeis.org/A089984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1) = a(2) = a(3) = a(4) = 1, a(n) = a(n-4)*a(n-2) -
 *            a(n-3)*a(n-1) for n>4
 * Range:     i8 n <= 24, i16 n <= 26, i32 n <= 28, i64 n <= 30, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle out of bounds – behaviour is undefined for n < 1, but
       returning 0 keeps the function defined. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoisation. */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t   cache_size = 0;       /* size of allocated array */
    static idx_t   last_computed = 0;    /* largest n that has been computed */

    /* Grow cache if necessary */
    if (n > cache_size) {
        idx_t new_size = cache_size ? cache_size * 2 : 4;
        while (new_size < n)
            new_size *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) /* allocation failure */
            return (seq_t)0;
        cache = tmp;
        cache_size = new_size;
    }

    /* Compute missing values up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        idx_t idx = i - 1;                /* zero‑based index for cache */
        if (i <= 4) {
            cache[idx] = (seq_t)1;
        } else {
           /* a(i) = a(i-4)*a(i-2) - a(i-3)*a(i-1) */
            cache[idx] = cache[i-5] * cache[i-3] - cache[i-4] * cache[i-2];
        }
    }
    last_computed = n;
    return cache[n - 1];
}
