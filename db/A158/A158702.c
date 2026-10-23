/*
 * A158702  a(n) = Hermite(n, 19).
 * https://oeis.org/A158702
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Hermite(n, 19). Recurrence: a(0) = 1, a(1) = 38, a(n) =
 *            38*a(n-1) - 2*(n-1)*a(n-2) (n >= 2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 12, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoization */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return 0;                /* allocation failed */
        cache_cap = 2;
        cache[0] = 1;
        cache[1] = 38;
    }

    /* return already computed value */
    if ((idx_t)n < cache_cap) return cache[n];

    /* extend cache to n+1 if necessary */
    size_t new_cap = (size_t)(n + 1);
    if (new_cap > cache_cap) {
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                /* allocation failed */
        cache = tmp;
    }

    /* compute required terms */
    for (idx_t i = cache_cap; i <= n; ++i) {
        if (i >= 2) {
            cache[i] = (seq_t)(38 * cache[i - 1] -
                              (seq_t)(2 * (i - 1)) * cache[i - 2]);
        }
    }
    cache_cap = new_cap;
    return cache[n];
}
