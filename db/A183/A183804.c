/*
 * A183804  1/16 the number of (n+1) X 3 binary arrays with no 2 X 2 subblock
 *          being a reflection across the shared element pair of any
 *          horizontal or vertical neighbor.
 * https://oeis.org/A183804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2) - 15*a(n-3); Initial values: a(1) =
 *            3, a(2) = 13, a(3) = 55
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 30, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* array of computed terms, 1‑based */
    static idx_t cache_size = 0;     /* allocated size (including index 0) */
    static idx_t cache_count = 0;    /* largest index that has been computed */

    if (n == 0) {                     /* not used, but safe for completeness */
        return (seq_t)0;
    }

    /* Ensure the cache is large enough for index n */
    if (n + 1 > cache_size) {
        idx_t new_size = (n + 1 > 4) ? (n + 1) : 4;            /* at least 4 */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        cache_size = new_size;
    }

    /* initialise first three terms if necessary */
    if (cache_count < 3) {
        if (cache_count < 1) {
            cache[1] = (seq_t)3;
            cache_count = 1;
        }
        if (cache_count < 2) {
            cache[2] = (seq_t)13;
            cache_count = 2;
        }
        if (cache_count < 3) {
            cache[3] = (seq_t)55;
            cache_count = 3;
        }
    }

    /* compute forward until the requested index */
    while (cache_count < n) {
        ++cache_count;
        cache[cache_count] =
            (seq_t)3 * cache[cache_count - 1] +
            (seq_t)9 * cache[cache_count - 2] -
            (seq_t)15 * cache[cache_count - 3];
    }

    return cache[n];
}
