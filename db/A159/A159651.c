/*
 * A159651  Numerator of Hermite(n, 13/19).
 * https://oeis.org/A159651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 26*a(n-1) - 722*(n-1)*a(n-2), a(0) = 1, a(1) = 26
 * Range:     i8 n <= 2, i16 n <= 2, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation: cache[i] == a(i) for i < cache_size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* next index to be computed */

    if (cache == NULL) {                          /* first call */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) exit(1);                      /* allocation failed */

        cache[0] = 1;                             /* a(0) */
        if (n >= 1) cache[1] = 26;                /* a(1) */

        for (idx_t i = 2; i <= n; ++i) {          /* fill up to n */
            seq_t term1 = (seq_t)26 * cache[i-1];
            seq_t term2 = (seq_t)722 * (seq_t)(i-1) * cache[i-2];
            cache[i] = term1 - term2;
        }
        cache_size = n + 1;
        return cache[n];
    }

    if (n < cache_size)                          /* already computed */
        return cache[n];

    /* extend cache to include index n */
    idx_t old_size = cache_size;
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!cache) exit(1);

    for (idx_t i = old_size; i <= n; ++i) {
        seq_t term1 = (seq_t)26 * cache[i-1];
        seq_t term2 = (seq_t)722 * (seq_t)(i-1) * cache[i-2];
        cache[i] = term1 - term2;
    }
    cache_size = n + 1;
    return cache[n];
}
