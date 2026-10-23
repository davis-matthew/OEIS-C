/*
 * A160315  Numerator of Hermite(n, 17/31).
 * https://oeis.org/A160315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0) = 1; a(1) = 34; a(n+2) = 34*a(n+1) - 1922*(n+1)*a(n) (for n
 *            ≥ 0)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache of sequence values   */
    static idx_t  max_n = 0;         /* highest index cached       */

    /* Allocate or grow cache to at least n+1 elements */
    if (cache == NULL) {
        /* First request: allocate array for n+1 elements */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* out of memory → zero */
    }
    else if (n > max_n) {
        /* Expand cache to accommodate n */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* out of memory → zero */
        cache = tmp;
    }

    /* Initialize base terms if not already done */
    if (max_n == 0) {
        cache[0] = (seq_t)1;
    }
    if (max_n < 1 && n >= 1) {
        cache[1] = (seq_t)34;
    }

    /* Compute new terms up to n, if needed */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t term = (seq_t)34 * cache[i - 1]
                     - (seq_t)1922 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term;
    }
    max_n = (n > max_n) ? n : max_n;

    /* Return the requested value */
    return cache[n];
}
