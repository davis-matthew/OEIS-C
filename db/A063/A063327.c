/*
 * A063327  Dimension of the space of weight n cuspidal newforms for Gamma_1(
 *          54 ).
 * https://oeis.org/A063327
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n > 6 initial values: a(2) =
 *            -1, a(3) = 22, a(4) = 42, a(5) = 64, a(6) = 86
 * Range:     i8 n <= 7, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 2) return 0;                    /* sequence undefined below offset */

    static seq_t *cache = NULL;             /* memoized values */
    static idx_t  cache_max = 1;            /* highest index computed */
    static size_t cache_cap = 0;            /* capacity of cache array */

    /* enlarge cache storage if necessary */
    if (n > cache_max) {
        size_t needed = (size_t)(n + 1);
        if (cache_cap < needed) {
            size_t new_cap = cache_cap ? cache_cap * 2 : needed;
            if (new_cap < needed) new_cap = needed;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failed, return 0 */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_max + 1; i <= n; ++i) {
            if (i == 2)       cache[i] = -1;
            else if (i == 3)  cache[i] = 22;
            else if (i == 4)  cache[i] = 42;
            else if (i == 5)  cache[i] = 64;
            else if (i == 6)  cache[i] = 86;
            else                cache[i] = cache[i-1] + cache[i-3] - cache[i-4];
        }
        cache_max = n;
    }

    return cache[n];
}
