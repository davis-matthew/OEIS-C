/*
 * A208264  Number of n X 3 0..1 arrays with new values 0..1 introduced in row
 *          major order and no element equal to more than two of its immediate
 *          leftward or upward or left-upward diagonal neighbors.
 * https://oeis.org/A208264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) - 2*a(n-3) for n>=4, with initial
 *            values a(1)=4, a(2)=25, a(3)=161
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;      /* number of terms stored in cache */

    if (n == 0) return 0;            /* undefined, safe guard */

    if (n <= cache_len)             /* already computed */
        return cache[n-1];

    /* Ensure base values are present */
    if (cache_len == 0) {
        cache_len = 3;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 4;        /* a(1) */
        cache[1] = 25;       /* a(2) */
        cache[2] = 161;      /* a(3) */
        if (n <= cache_len)
            return cache[n-1];
    }

    /* Grow the cache to accommodate n */
    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)            /* allocation failed; caller should handle */
            return 0;
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t v = 6 * cache[i-2] + 3 * cache[i-3] - 2 * cache[i-4];
            cache[i-1] = v;
        }
        cache_len = n;
    }

    return cache[n-1];
}
