/*
 * A207144  Number of (n+1) X 4 0..2 arrays with every 2 X 2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A207144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3) for n>10 Initial values:
 *            a(1)=80, a(2)=56, a(3)=157, a(4)=110, a(5)=311, a(6)=218,
 *            a(7)=619, a(8)=434, a(9)=1235, a(10)=866
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 52, u64 n <= 116, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoization array, indices start at 0 */
    static size_t last = 0;       /* largest indexed term that has been computed */

    if (n == 0) return (seq_t)0;  /* undefined for n<1, return 0 */

    /* First call: allocate base cache and set the first 10 terms */
    if (cache == NULL) {
        size_t cap = 11;                                 /* indices 0..10 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;                     /* allocation failed */

        cache[1] = (seq_t)80;
        cache[2] = (seq_t)56;
        cache[3] = (seq_t)157;
        cache[4] = (seq_t)110;
        cache[5] = (seq_t)311;
        cache[6] = (seq_t)218;
        cache[7] = (seq_t)619;
        cache[8] = (seq_t)434;
        cache[9] = (seq_t)1235;
        cache[10] = (seq_t)866;

        last = 10;
    }

    /* Grow the cache if the requested index is beyond current size */
    if ((size_t)n + 1 > last + 1) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* cannot grow */
        cache = tmp;
        for (size_t i = last + 1; i < new_cap; ++i) cache[i] = (seq_t)0;
    }

    /* Compute any missing terms up to n using the recurrence */
    if (n > last) {
        for (size_t i = last + 1; i <= (size_t)n; ++i) {
            /* recurrence valid for i > 10; values < 11 are already set */
            cache[i] = cache[i-1] + (seq_t)2 * cache[i-2] - (seq_t)2 * cache[i-3];
        }
        last = (size_t)n;
    }

    return cache[(size_t)n];
}
