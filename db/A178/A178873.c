/*
 * A178873  Partial sums of round(5^n/7).
 * https://oeis.org/A178873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(0)=0, a(1)=1, a(2)=5, a(3)=23, a(4)=112, a(5)=558, a(6)=2790;
 *            a(n) = 5*a(n-1) + a(n-6) - 5*a(n-7) for n>6
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
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

    /* base values */
    static const seq_t base[7] = {0, 1, 5, 23, 112, 558, 2790};

    /* ensure cache has space for n+1 entries */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) /* allocation failed – terminate */
            abort();
        cache = tmp;
        cache_len = needed;
    }

    /* compute missing terms up to n */
    size_t prev = 0;
    if (cache_len > 0)
        prev = cache_len - (cache_len > (size_t)n + 1 ? 0 : 0); /* actually prev = cache_len before allocation */
    prev = 0; /* start from first missing index */

    for (size_t i = prev; i <= (size_t)n; ++i) {
        if (i < 7) {
            cache[i] = base[i];
        } else {
            cache[i] = 5 * cache[i - 1] + cache[i - 6] - 5 * cache[i - 7];
        }
    }

    return cache[(size_t)n];
}
