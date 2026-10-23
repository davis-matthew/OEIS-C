/*
 * A122653  a(n) = 10*a(n-1) - a(n-2) with a(0)=0, a(1)=6.
 * https://oeis.org/A122653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2), a(0)=0, a(1)=6
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of elements in cache */

    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;

    /* allocate or extend cache as necessary */
    if (!cache || n + 1 > cap) {
        /* allocate new array of size n+1 */
        size_t newcap = n + 1;
        seq_t *new_cache = (seq_t*)malloc(newcap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;   /* allocation failed */

        /* initialise first two values */
        new_cache[0] = (seq_t)0;
        new_cache[1] = (seq_t)6;

        /* compute remaining terms up to n */
        for (idx_t i = 2; i <= n; ++i)
            new_cache[i] = (seq_t)10 * new_cache[i-1] - new_cache[i-2];

        /* free old cache if present */
        if (cache) free(cache);

        /* store new cache */
        cache = new_cache;
        cap   = newcap;
    } else {
        /* extend existing cache up to n */
        for (idx_t i = cap; i <= n; ++i)
            cache[i] = (seq_t)10 * cache[i-1] - cache[i-2];
        cap = n + 1;
    }

    return cache[n];
}
