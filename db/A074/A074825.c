/*
 * A074825  Binomial transform of reflected pentanacci numbers A074062: a(n) =
 *          Sum_{k=0..n} binomial(n,k)*A074062(k).
 * https://oeis.org/A074825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 20, i32 n <= 41, i64 n <= 82, i128 n <= 167,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Recurrence: a(0)=5, a(1)=4, a(2)=2, a(3)=-2, a(4)=-10
   a(n) = 4*a(n-1) - 7*a(n-2) + 6*a(n-3) - 3*a(n-4) + 2*a(n-5)  (n≥5) */

/* External types defined by the build system              */
seq_t a(idx_t n)
{
    /* static cache for memoization                               */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialise cache with the first five terms                 */
    if (cache_size == 0) {
        cache_size = 5;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[0] = (seq_t)5;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)-2;
        cache[4] = (seq_t)-10;
    }

    /* return if already computed                                 */
    if (n < cache_size) {
        return cache[n];
    }

    /* extend cache to accommodate up to n                        */
    if ((idx_t)cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache,
                                      (size_t)new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;

        /* compute missing terms using recurrence                 */
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] =
                (seq_t)4 * cache[i - 1] -
                (seq_t)7 * cache[i - 2] +
                (seq_t)6 * cache[i - 3] -
                (seq_t)3 * cache[i - 4] +
                (seq_t)2 * cache[i - 5];
        }
        cache_size = new_size;
    }

    return cache[n];
}
