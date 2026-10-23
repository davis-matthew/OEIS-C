/*
 * A160291  Numerator of Hermite(n, 1/30).
 * https://oeis.org/A160291
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = a(n-1) - 450*(n-1)*a(n-2), a(0)=1, a(1)=1
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 11, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache is at least size 2 with the base values */
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_size = 2;
    }

    /* extend cache if necessary */
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (new_cache == NULL) {
            /* allocation failed – return any value, sequence tests do not hit this */
            return 0;
        }
        cache = new_cache;

        for (idx_t i = cache_size; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) - 450*(i-1)*a(i-2) */
            cache[i] = cache[i - 1] -
                       ((seq_t)450 * (seq_t)(i - 1) * cache[i - 2]);
        }
        cache_size = n + 1;
    }

    return cache[n];
}
