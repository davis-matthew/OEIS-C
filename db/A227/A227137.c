/*
 * A227137  Positive solutions of the Pell equation x^2 - 89*y^2 = -1.
 *          Solutions x = 500*a(n).
 * https://oeis.org/A227137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*500001*a(n-1) - a(n-2); a(0)=1, a(1)=1000003
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Cached values are stored in a static array.
       cache_len keeps the number of elements already computed
       (cache[0..cache_len-1] are valid). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_len = 0;      /* number of initialized entries */

    /* Constants for the recurrence */
    const seq_t C   = (seq_t)1000002;     /* 2 * 500001 */
    const seq_t A0  = (seq_t)1;
    const seq_t A1  = (seq_t)1000003;

    /* Ensure we have enough space for index n */
    if (n >= cache_len) {
        ensure_cache(n, &cache, &cache_cap);

        /* Compute and store values from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i == 0)
                cache[i] = A0;
            else if (i == 1)
                cache[i] = A1;
            else
                cache[i] = C * cache[i-1] - cache[i-2];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
