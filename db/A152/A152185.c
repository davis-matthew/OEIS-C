/*
 * A152185  a(n) = -3*a(n-1) + 5*a(n-2), n > 1; a(0)=1, a(1)=-5.
 * https://oeis.org/A152185
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) + 5*a(n-2), a(0)=1, a(1)=-5
 * Range:     i8 n <= 3, i16 n <= 7, i32 n <= 14, i64 n <= 30, i128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements stored in cache */

    /* enlarge cache if necessary */
    if (n + 1 > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory -> terminate */
        cache = tmp;

        /* initialise first elements if the cache is newly created */
        if (cache_size == 0) {
            cache[0] = (seq_t)1;
            if (n >= 1) cache[1] = (seq_t)-5;
            cache_size = (n >= 1) ? 2 : 1;
        } else if (cache_size == 1) {
            cache[1] = (seq_t)-5;
            cache_size = 2;
        }

        /* compute the remaining terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = -((seq_t)3 * cache[i - 1]) + ((seq_t)5 * cache[i - 2]);
        }
        cache_size = n + 1;
    }

    return cache[n];
}
