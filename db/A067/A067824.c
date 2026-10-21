/*
 * A067824  a(1) = 1; for n > 1, a(n) = 1 + Sum_{0 < d < n, d|n} a(d).
 * https://oeis.org/A067824
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = 1 + sum_{0 < d < n, d|n} a(d)
 * Range:     u8 n <= 119, u16 n <= 5759, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* the function that computes the A067824 sequence */
seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of terms currently stored */
    static idx_t cache_cap  = 0;   /* allocated capacity of cache */

    /* grow the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return 0;          /* allocation failed – return 0 (undefined) */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 1;                      /* a(1) = 1 */
        } else {
            seq_t sum = 0;
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d == 0) {
                    idx_t d1 = d;
                    idx_t d2 = i / d;
                    if (d1 < i) sum += cache[d1 - 1];
                    if (d2 != d1 && d2 < i) sum += cache[d2 - 1];
                }
            }
            cache[i - 1] = 1 + sum;
        }
    }

    cache_size = n;
    return cache[n - 1];
}
