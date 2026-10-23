/*
 * A160150  Numerator of Hermite(n, 22/27).
 * https://oeis.org/A160150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=44; for n ≥ 2 a(n) = 44·a(n−1) − 1458·(n−1)·a(n−2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated number of entries */
    static idx_t  last  = 0;   /* highest computed index */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cap = (n < 1 ? 2 : (size_t)n + 1);   /* at least two slots */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;         /* allocation failure */
        cache[0] = (seq_t)1;
        if (cap > 1)
            cache[1] = (seq_t)44;
        last = 1;
    }

    /* handle the first two indices directly */
    if (n < 2)
        return cache[n];

    /* enlarge cache when necessary */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp == NULL) return 0;          /* allocation failure */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t t_prev  = cache[i - 1];
        seq_t t_prev2 = cache[i - 2];
        seq_t factor  = ((seq_t)1458 * (seq_t)(i - 1));
        cache[i] = ((seq_t)44 * t_prev) - (factor * t_prev2);
    }
    last = n;
    return cache[n];
}
