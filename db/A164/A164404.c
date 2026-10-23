/*
 * A164404  Number of binary strings of length n with no substrings equal to
 *          0010 or 1011.
 * https://oeis.org/A164404
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 40, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4)  (n ≥ 8)
   Base values: a(4)=14, a(5)=24, a(6)=41, a(7)=70; undefined for n<4. */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* No defined value for indices below the offset. */
    if (n < 4) {
        return 0;
    }

    /* Static cache for memoisation. */
    static seq_t *cache      = NULL;   /* array of a(i) values */
    static size_t cache_cap  = 0;      /* allocated capacity (>= n+1) */
    static idx_t  cache_max  = 0;      /* largest index that has been computed */

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 4) {
            cache[i] = 14;
        } else if (i == 5) {
            cache[i] = 24;
        } else if (i == 6) {
            cache[i] = 41;
        } else if (i == 7) {
            cache[i] = 70;
        } else { /* i >= 8 */
            seq_t val = (seq_t)2 * cache[i - 1];
            val -= (seq_t)2 * cache[i - 3];
            val += (seq_t)2 * cache[i - 4];
            cache[i] = val;
        }
        cache_max = i;
    }

    return cache[n];
}
