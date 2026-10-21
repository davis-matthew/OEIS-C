/*
 * A229574  Number of defective 4-colorings of an n X 4 0..3 array connected
 *          horizontally, vertically, diagonally and antidiagonally with
 *          exactly one mistake, and colors introduced in row-major 0..3
 *          order.
 * https://oeis.org/A229574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 13*a(n-2) + 12*a(n-3) - 4*a(n-4) for n > 7 *
 *            Initial values: * a(1) = 6 * a(2) = 80 * a(3) = 171 * a(4) = 312
 *            * a(5) = 568 * a(6) = 1064 * a(7) = 2064 * * The function
 *            memoizes results in a static array, so that calls may be * in
 *            any order and each value is computed at most once
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cache[0] unused; cache[1..] holds a(1..) */
    static idx_t last = 0;        /* highest index computed so far */
    static size_t cap  = 0;       /* allocated size of cache */

    if (n <= 0)
        return (seq_t)0;          /* undefined for n <= 0, return 0 */

    /* Ensure the cache is large enough. */
    if (cap < n + 1) {
        size_t newcap = cap ? cap * 2 : 8;
        if (newcap < n + 1) newcap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        /* zero-initialize new slots (needed only for safety). */
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = (seq_t)0;
        cap = newcap;
    }

    /* Initialise the base values once. */
    if (last < 7) {
        if (last < 1) cache[1] = (seq_t)6;
        if (last < 2) cache[2] = (seq_t)80;
        if (last < 3) cache[3] = (seq_t)171;
        if (last < 4) cache[4] = (seq_t)312;
        if (last < 5) cache[5] = (seq_t)568;
        if (last < 6) cache[6] = (seq_t)1064;
        if (last < 7) cache[7] = (seq_t)2064;
        last = 7;
    }

    /* Compute up to n if necessary. */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* Compute 6*a(i-1) + 12*a(i-3) - (13*a(i-2) + 4*a(i-4)) */
            seq_t sum1 = 6 * cache[i - 1] + 12 * cache[i - 3];
            seq_t sum2 = 13 * cache[i - 2] + 4 * cache[i - 4];
            cache[i]   = sum1 - sum2;     /* subtraction is safe: sum1 >= sum2 */
        }
        last = n;
    }

    return cache[n];
}
