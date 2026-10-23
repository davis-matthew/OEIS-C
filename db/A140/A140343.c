/*
 * A140343  a(n)=4a(n-1)-7a(n-2)+6a(n-3)-3a(n-4), n>4.
 * https://oeis.org/A140343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-1) - 7*a(n-2) + 6*a(n-3) - 3*a(n-4), a(0) = a(1) =
 *            a(2) = a(3) = 0, a(4) = 1
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 40, i64 n <= 40, i128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* values for 0 <= n < 5 */
    if (n < 4) return (seq_t)0;
    if (n == 4) return (seq_t)1;

    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 4; /* up to which term we have computed */

    /* ensure cache has space for indices up to n */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* on allocation failure */
        /* initialise base values */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)1;
    } else if (n + 1 > cache_size) {
        cache_size = n + 1;
        cache = (seq_t *)realloc(cache, (size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        /* If we are expanding beyond the initial base, keep base values */
        if (cache_size >= 5) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)0;
            cache[3] = (seq_t)0;
            cache[4] = (seq_t)1;
        }
    }

    /* compute missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        cache[i] =
            (seq_t)4 * cache[i - 1] -
            (seq_t)7 * cache[i - 2] +
            (seq_t)6 * cache[i - 3] -
            (seq_t)3 * cache[i - 4];
    }
    last_computed = n;
    return cache[n];
}
