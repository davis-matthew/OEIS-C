/*
 * A198862  Sum of the n-th antidiagonal in the triangle A192011.
 * https://oeis.org/A198862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 26, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = -1
 *           a(1) = a(2) = a(3) =  2
 *           a(n) = a(n-1) - a(n-4)  for n > 3
 *
 * The function uses a static cache so that repeated calls, even out of
 * order, are efficient.  Recurrence values are computed on demand.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;    /* number of allocated elements */

    /* ensure that cache[n] is available */
    if ((size_t)n >= cache_size) {
        size_t new_size = cache_size ? cache_size << 1 : 4;
        if (new_size <= (size_t)n) new_size = (size_t)n + 1;

        /* allocate new larger array */
        seq_t *tmp = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* fallback on allocation failure */

        /* copy previously cached values */
        for (size_t i = 0; i < cache_size; ++i) {
            tmp[i] = cache[i];
        }

        /* initialise new entries using the recurrence */
        for (size_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                tmp[i] = (seq_t)-1;
            } else if (i == 1 || i == 2 || i == 3) {
                tmp[i] = (seq_t)2;
            } else {
                tmp[i] = tmp[i - 1] - tmp[i - 4];
            }
        }

        /* replace old cache */
        if (cache) free(cache);
        cache     = tmp;
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
