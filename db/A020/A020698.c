/*
 * A020698  a(n) = 5*a(n-1) - 2*a(n-2), with a(0)=2, a(1)=9.
 * https://oeis.org/A020698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 2*a(n-2); a(0)=2, a(1)=9
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 28, u128 n <= 58,
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
    static size_t cache_len = 0;
    static size_t max_computed = 0;

    /* initial allocation and base values */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        max_computed = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > cache_len) {
        cache_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    }

    /* compute missing terms up to n */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            cache[i] = (seq_t)5 * prev - (seq_t)2 * prev2;
        }
        max_computed = n;
    }

    return cache[n];
}
