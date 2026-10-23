/*
 * A160299  Numerator of Hermite(n, 1/31).
 * https://oeis.org/A160299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1, a(1)=2, a(n+2) = 2*a(n+1) - 1922*(n+1)*a(n) (for n>=0)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cached_max = (idx_t)-1;  /* indicates cache is empty */

    /* initialise cache with first two terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        if (!cache) return 0;            /* allocation failed; unlikely */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cached_max = 1;
    }

    /* if requested index is already in cache, return it */
    if (n <= cached_max) return cache[n];

    /* expand cache to size n+1 */
    size_t new_size = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;  /* allocation failed; unlikely */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        /* term1 = 2 * a(i-1) */
        seq_t term1 = cache[i - 1] * (seq_t)2;

        /* term2 = 1922 * (i-1) * a(i-2)   (use casting to seq_t) */
        seq_t mul1 = cache[i - 2] * (seq_t)(i - 1);
        seq_t term2 = mul1 * (seq_t)1922;

        /* a(i) = term1 - term2 */
        cache[i] = term1 - term2;
    }

    cached_max = n;
    return cache[n];
}
