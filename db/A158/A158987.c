/*
 * A158987  Numerator of Hermite(n, 3/7).
 * https://oeis.org/A158987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 6*a(n-1) - 98*(n-1)*a(n-2); a(0)=1, a(1)=6 Recurrence
 *            from OEIS A158987 (Numerator of Hermite(n, 3/7))
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 8, i64 n <= 14, i128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;

    /* Initialize cache on first call */
    if (cache_size == 0) {
        cache_cap  = 2;
        cache      = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0]   = (seq_t)1;
        cache[1]   = (seq_t)6;
        cache_size = 2;
    }

    /* Grow cache if necessary */
    if (n >= cache_size) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t term = 6 * cache[i-1] -
                         98 * ((seq_t)(i-1)) * cache[i-2];
            cache[i] = term;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
