/*
 * A174171  A generalized Chebyshev transform of the Motzkin numbers A001006.
 * https://oeis.org/A174171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + Sum_{k=0..n-2} a(k) * a(n-2-k)
 *            initial values: a(0)=1, a(1)=1 This implements the recursive
 *            definition from OEIS A174171
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_len = 2;                       /* we need a[0] and a[1] */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;                /* on allocation failure */
        cache[0] = 1;
        cache[1] = 1;
    }

    /* compute all missing terms up to n */
    if (n >= cache_len) {
        size_t old_len = cache_len;
        cache_len = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
        if (!tmp) return 0;                /* on allocation failure */
        cache = tmp;

        for (idx_t i = old_len; i <= n; ++i) {
            seq_t sum = 0;
            /* convolution term: sum_{k=0..i-2} a(k) * a(i-2-k) */
            for (idx_t k = 0; k <= i - 2; ++k) {
                sum += cache[k] * cache[i - 2 - k];
            }
            cache[i] = cache[i - 1] + cache[i - 2] * 2 + sum;
        }
    }

    return cache[n];
}
