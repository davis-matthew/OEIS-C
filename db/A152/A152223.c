/*
 * A152223  a(n) = -4*a(n-1) + 6*a(n-2) for n > 1 with a(0) = 1 and a(1) = -6.
 * https://oeis.org/A152223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) + 6*a(n-2) initial values: a(0) = 1, a(1) = -6
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 13, i64 n <= 26, i128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache    = NULL;
    static idx_t  cache_sz = 0;

    /* grow cache to accommodate up to n */
    if (n >= cache_sz) {
        /* new size (n+1 ensures indices 0..n are valid) */
        idx_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure: return 0 */
        cache    = tmp;
        cache_sz = new_sz;
    }

    /* initialise missing entries */
    for (idx_t i = 0; i < cache_sz; ++i) {
        if (i == 0)                          /* a(0) */
            cache[i] = (seq_t)1;
        else if (i == 1)                     /* a(1) */
            cache[i] = (seq_t)-6;
        else if (i >= 2) {                   /* recurrence */
            cache[i] = -4 * cache[i-1] + 6 * cache[i-2];
        }
    }

    return cache[n];
}
