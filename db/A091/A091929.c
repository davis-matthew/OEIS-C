/*
 * A091929  Expansion of (1-6*x)/(1-6*x-11*x^2).
 * https://oeis.org/A091929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1)+11*a(n-2), a(0)=1, a(1)=0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity */
    static idx_t cache_size = 0;  /* number of terms stored (>= cache_cap) */

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache_size = 2;
    }

    /* return value if already computed */
    if (n < cache_size)
        return cache[n];

    /* enlarge cache if necessary */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = (idx_t)new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)6 + cache[i - 2] * (seq_t)11;
    }
    cache_size = n + 1;

    return cache[n];
}
