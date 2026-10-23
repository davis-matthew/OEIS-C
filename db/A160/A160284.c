/*
 * A160284  Numerator of Hermite(n, 22/29).
 * https://oeis.org/A160284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(0)=1; a(1)=44; a(n)=44*a(n-1)-1682*(n-1)*a(n-2) (n>=2)
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0; /* allocated size */
    static idx_t cache_len = 0; /* number of valid entries */

    /* enlarge storage if necessary */
    if (n >= cache_len) {
        size_t needed = (size_t)n + 1;
        if (cache_cap < needed) {
            size_t new_cap = (cache_cap ? cache_cap * 2 : 16);
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) exit(1); /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1) {
                cache[1] = (seq_t)44;
            } else {
                cache[i] = (seq_t)44 * cache[i-1]
                           - (seq_t)1682 * (seq_t)(i-1) * cache[i-2];
            }
        }
        cache_len = n + 1;
    }
    return cache[n];
}
