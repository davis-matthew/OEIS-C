/*
 * A161158  a(n) = A003696(n+1)/A001353(n+1).
 * https://oeis.org/A161158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 34*a(n-2) + 14*a(n-3) - a(n-4); initial
 *            values: a(0)=1, a(1)=14, a(2)=161, a(3)=1792
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* allocated cache for computed terms   */
    static idx_t cache_len = 0;      /* number of already computed terms     */

    /* ensure at least the first four terms are stored */
    if (cache_len == 0) {
        size_t init_len = 4;
        cache = (seq_t *)realloc(cache, init_len * sizeof(seq_t));
        if (!cache) {
            /* allocation failed – return 0 as a safe fallback */
            return 0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)14;
        cache[2] = (seq_t)161;
        cache[3] = (seq_t)1792;
        cache_len = 4;
    }

    /* if n is within the cached range, return directly */
    if (n < cache_len) {
        return cache[n];
    }

    /* enlarge cache to hold terms up to n */
    size_t new_len = (size_t)(n + 1);
    if (new_len < 4) {
        new_len = 4;                /* never shrink below initial size */
    }
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – return 0 as a safe fallback */
        return 0;
    }
    cache = tmp;
    cache_len = (idx_t)new_len;

    /* compute missing terms using the recurrence */
    for (idx_t i = 4; i <= n; ++i) {
        cache[i] = (seq_t)(14 * cache[i-1]
                           - 34 * cache[i-2]
                           + 14 * cache[i-3]
                           - cache[i-4]);
    }

    return cache[n];
}
