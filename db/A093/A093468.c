/*
 * A093468  a(1) = 1, a(2) = 2; for n >= 2, a(n+1) = a(n) + Sum {a(n)-a(i), i
 *          = 1 to n}.
 * https://oeis.org/A093468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - (n-1)*a(n-2); a(1)=1, a(2)=2, a(3)=3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    /* Static cache for memoisation (1‑based indices). */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with the first three terms on first call. */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;               /* insufficient memory */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
    }

    /* Extend cache up to the required term. */
    if (n > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) return (seq_t)0;               /* insufficient memory */
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            /* a(i) = i * a(i-1) - (i-1) * a(i-2) */
            seq_t val = (seq_t)i * cache[i - 2] - (seq_t)(i - 1) * cache[i - 3];
            cache[i - 1] = val;
        }
        cache_len = n;
    }

    return cache[n - 1];
}
