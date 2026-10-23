/*
 * A021394  Expansion of 1/((1-x)(1-3x)(1-4x)(1-11x)).
 * https://oeis.org/A021394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 107*a(n-2) + 221*a(n-3) - 132*a(n-4) for n >
 *            3 a(0) = 1, a(1) = 19, a(2) = 254, a(3) = 3014 This recurrence
 *            follows from the generating function
 *            1/((1-x)(1-3x)(1-4x)(1-11x))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation (does not depend on call order) */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of computed terms stored. */
    static size_t cache_cap = 0;   /* allocated capacity of cache. */

    /* ensure cache is large enough for indices up to n */
    if (cache_len <= n) {
        /* grow capacity to at least n+1 */
        size_t new_cap = (cache_cap ? cache_cap : 1);
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;

        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;

        /* compute missing terms up to n */
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0)          cache[0] = (seq_t)1;
            else if (k == 1)      cache[1] = (seq_t)19;
            else if (k == 2)      cache[2] = (seq_t)254;
            else if (k == 3)      cache[3] = (seq_t)3014;
            else                  cache[k] = 19 * cache[k-1]
                                 - 107 * cache[k-2]
                                 + 221 * cache[k-3]
                                 - 132 * cache[k-4];
        }
        cache_len = (size_t)n + 1;
    }

    return cache[n];
}
