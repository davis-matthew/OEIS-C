/*
 * A086901  a(1) = a(2) = 1; for n>2, a(n) = 4*a(n-1) + 3*a(n-2).
 * https://oeis.org/A086901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = a(2) = 1; for n>2, a(n) = 4*a(n-1) + 3*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                  /* undefined for n<1 but keep safe */

    /* static cache to memoize previously computed terms */
    static seq_t *cache      = NULL;     /* cache[i] holds a(i+1) */
    static idx_t cached_len  = 0;        /* number of terms currently stored */
    static idx_t cache_cap   = 0;        /* allocated capacity */

    /* initialise cache for the first two terms */
    if (cached_len == 0) {
        cache_cap = (n > 2) ? n : 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;          /* a(1) */
        cache[1] = (seq_t)1;          /* a(2) */
        cached_len = 2;
    }
    /* grow cache if the requested index exceeds capacity */
    else if (n > cache_cap) {
        cache_cap = n > cache_cap * 2 ? n : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
    }

    /* compute missing terms up to n */
    if (n > cached_len) {
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            /* cache[i-1] corresponds to a(i) */
            seq_t prev1 = cache[i - 2];  /* a(i-1) */
            seq_t prev2 = cache[i - 3];  /* a(i-2) */
            cache[i - 1] = prev1 * (seq_t)4 + prev2 * (seq_t)3;
        }
        cached_len = n;
    }

    return cache[n - 1];                  /* a(n)  (index is 1‑based) */
}
