/*
 * A159997  Numerator of Hermite(n, 19/24).
 * https://oeis.org/A159997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 19*a(n-1) - 288*(n-1)*a(n-2), a(0)=1, a(1)=19
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 7, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)19;

    /* Cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t last_computed = 1;  /* last index for which cache[i] is valid */

    /* Initialise cache on first call that needs it */
    if (!cache) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                       /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)19;
    }

    /* Grow cache if necessary */
    if ((idx_t)(n + 1) > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            abort();                                 /* allocation failure */
        }
        cache = tmp;
        cache_size = new_size;
    }

    /* Fill cache up to requested index */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        /* a(i) = 19 * a(i-1) - 288 * (i-1) * a(i-2) */
        seq_t term   = (seq_t)19 * cache[i - 1];
        seq_t sub    = (seq_t)288 * (seq_t)(i - 1) * cache[i - 2];
        cache[i] = term - sub;
    }

    last_computed = n;
    return cache[n];
}
