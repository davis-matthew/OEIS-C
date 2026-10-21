/*
 * A090132  Expansion of (1+2*x)/(1+2*x+2*x^2).
 * https://oeis.org/A090132
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = -a(n-1) + 2*a(n-3) Initial values: a(0)=1, a(1)=0,
 *            a(2)=-2 The sequence can be computed in O(n) time per call with
 *            memoisation
 * Range:     i8 n <= 13, i16 n <= 29, i32 n <= 46, i64 n <= 46, i128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static buffer for memoisation */
    static seq_t *cache = NULL;          /* array holding computed values   */
    static idx_t  cache_len = 0;         /* number of values stored          */
    static idx_t  cache_cap = 0;         /* allocated capacity (in elements) */

    /* initialise first 3 values */
    if (cache_len == 0) {
        cache_cap = 3;
        cache_len = 3;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)-2;
    }

    /* grow cache if necessary and compute any missing terms */
    while (cache_len <= n) {
        /* ensure capacity */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }
        /* compute next term using the recurrence */
        if (cache_len >= 3) {
            cache[cache_len] = -cache[cache_len - 1] + (seq_t)2 * cache[cache_len - 3];
        }
        cache_len++;
    }

    return cache[n];
}
