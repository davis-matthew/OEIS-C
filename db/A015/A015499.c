/*
 * A015499  a(0)=1, a(1)=11, a(n) = sum_{k=0}^{k=n-1} 11^k a(k).
 * https://oeis.org/A015499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(n)=\sum_{k=0}^{n-1} 11^k a(k) (n>1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy, incremental cache */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static idx_t  max_computed = (idx_t)-1;
    static seq_t  pow11 = 0;   /* 11^k where k == max_computed */

    /* initialise on first call */
    if (max_computed == (idx_t)-1) {
        max_computed = 0;
        cache_len   = 1;
        cache       = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0]    = (seq_t)1;                /* a(0) */
        pow11       = (seq_t)1;                /* 11^0 */
    }

    /* grow cache if needed */
    if ((size_t)n + 1 > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, ((size_t)n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* allocation failure */
        cache     = tmp;
        cache_len = (size_t)n + 1;
    }

    /* compute missing terms */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        if (k == 1) {
            cache[1] = (seq_t)11;
            pow11    = pow11 * (seq_t)11;   /* pow11 = 11^1 now */
        } else if (k == 2) {
            /* cache[2] = cache[0] + 11^1 * cache[1] */
            cache[2] = cache[0] + pow11 * cache[1];
            pow11    = pow11 * (seq_t)11;   /* pow11 = 11^2 */
        } else {
            /* a(k) = (11^{k-1} + 1) * a(k-1) */
            cache[k] = (pow11 + (seq_t)1) * cache[k - 1];
            pow11    = pow11 * (seq_t)11;   /* pow11 = 11^k */
        }
    }
    max_computed = n;
    return cache[n];
}
