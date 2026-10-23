/*
 * A030065  a(4n)=n, a(4n+2)=a(4n)+a(4n+4), a(2n+1)=a(2n)+a(2n+2).
 * https://oeis.org/A030065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) (n≥8), a(0)=0, a(1)=1, a(2)=1, a(3)=2,
 *            a(4)=1, a(5)=4, a(6)=3, a(7)=5
 * Range:     u8 n <= 340, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated capacity */
    static size_t cache_len = 0;   /* number of initialized entries */

    /* Ensure the cache can hold at least n+1 elements, but at least 8 for
       the known base values. */
    idx_t need = (n > 7) ? n : 7;
    ensure_cache(need, &cache, &cache_cap);

    /* Initialize base values once. */
    if (cache_len < 8) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)1;
        cache[5] = (seq_t)4;
        cache[6] = (seq_t)3;
        cache[7] = (seq_t)5;
        cache_len = 8;
    }

    /* Compute any missing terms up to n. */
    if (cache_len <= n) {
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = (seq_t)2 * cache[i - 4] - cache[i - 8];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
