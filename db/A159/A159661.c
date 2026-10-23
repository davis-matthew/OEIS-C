/*
 * A159661  The general form of the recurrences are the a(j), b(j) and n(j)
 *          solutions of the 2 equations problem: 11*n(j) + 1 = a(j)*a(j) and
 *          13*n(j) + 1 = b(j)*b(j) with positive integer elements. the
 *          solutions of the 2 equations problem: 11*n(j) + 1 = a(j)*a(j);
 *          13*n(j) + 1 = b(j)*b(j); with integer numbers.
 * https://oeis.org/A159661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*a(n-1) - a(n-2); a(1) = 1, a(2) = 25 (offset 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Offset 1 – never called with n==0 in the test harness */
    if (n == 0) return 0;

    static seq_t *cache = NULL;   /* cached terms, 1‑based */
    static idx_t  last  = 0;      /* highest index computed so far */
    static size_t cap   = 0;      /* number of elements allocated */

    /* Grow the cache if we need more room for index n */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap : 2;
        while ((size_t)n + 1 > newcap) newcap <<= 1;   /* exponential growth */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the first two terms when needed */
    if (last < 1) {
        cache[1] = (seq_t)1;
        last = 1;
    }
    if (last < 2 && n >= 2) {
        cache[2] = (seq_t)25;
        last = 2;
    }

    /* Compute terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = 24 * cache[i - 1] - cache[i - 2];
    }
    last = n;

    return cache[n];
}
