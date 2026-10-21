/*
 * A081341  Expansion of exp(3*x)*cosh(3*x).
 * https://oeis.org/A081341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(n) = 6 * a(n-1) for all n ≥ 1 (equivalently a(n) = 6ⁿ
 *            / 2 for n > 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc/realloc */

seq_t a(idx_t n)
{
    /* Special case for the offset */
    if (n == 0)
        return (seq_t)1;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t  cache_cap = 0;   /* allocated size of cache          */
    static idx_t   cache_len = 0;   /* number of entries already set    */

    /* Ensure there is room for at least (n+1) elements */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialize base cases if the cache is empty */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (cache_len == 1) {
        /* a(1) = 3 */
        cache[1] = (seq_t)3;
        cache_len = 2;
    }

    /* Fill in any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)6;
    }
    cache_len = n + 1;

    return cache[n];
}
