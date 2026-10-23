/*
 * A160041  Positive numbers y such that y^2 is of the form x^2+(x+73)^2 with
 *          integer x.
 * https://oeis.org/A160041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 32, u64 n <= 70, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A160041
   Formula: a(n) = 6*a(n-3) - a(n-6)  (n > 6)
   Initial values:
      a(1) = 53, a(2) = 73, a(3) = 125,
      a(4) = 193, a(5) = 365, a(6) = 697
   The function memoizes computed terms to handle arbitrary call order
   in O(n) time and O(n) space, using only integer operations.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to remember previously computed terms */
    static seq_t *cache = NULL;          /* array of computed a(k) */
    static idx_t cached = 0;             /* number of terms stored in cache */

    /* initialisation for first call */
    if (cached == 0) {
        /* allocate space for the first 6 terms */
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        /* store the initial terms */
        cache[0] = (seq_t)53;
        cache[1] = (seq_t)73;
        cache[2] = (seq_t)125;
        cache[3] = (seq_t)193;
        cache[4] = (seq_t)365;
        cache[5] = (seq_t)697;
        cached = 6;
    }

    /* if requested term already in cache, return it immediately */
    if (n <= cached) {
        return cache[(size_t)(n - 1)];
    }

    /* expand cache to accommodate the new terms up to n */
    if (n > cached) {
        /* realloc to new size n */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (tmp == NULL) {
            /* allocation failed; return 0 as a safe error value */
            return (seq_t)0;
        }
        cache = tmp;
        /* compute missing terms starting from cached+1 up to n */
        for (idx_t k = cached + 1; k <= n; ++k) {
            size_t idx = (size_t)(k - 1);      /* zero‑based index in array */
            size_t idx_m3 = (size_t)(k - 4);   /* (k-3)-1 */
            size_t idx_m6 = (size_t)(k - 7);   /* (k-6)-1 */
            cache[idx] = cache[idx_m3] * (seq_t)6 - cache[idx_m6];
        }
        cached = n;
    }

    return cache[(size_t)(n - 1)];
}
