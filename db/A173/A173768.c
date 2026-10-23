/*
 * A173768  a(n) = (4*10^n - 31)/9.
 * https://oeis.org/A173768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*10^n - 31)/9 Recurrence used: a(1) = 1, a(n) =
 *            10*a(n-1) + 31 for n > 1 Cached computation for repeated calls
 *            and arbitrary n
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* offset starts at 1 */
    if (n < 1)
        return 0; /* undefined, but safe return */

    /* static cache: index i holds a(i), i >= 1 */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_last = 0; /* last computed index */

    /* Ensure cache can hold indices up to n */
    ensure_cache(n + 1, &cache, &cache_cap); /* we allocate n+1 slots, index 0 unused */

    /* Initialise first term if not yet done */
    if (cache_last == 0) {
        cache[1] = (seq_t)1;
        cache_last = 1;
    }

    /* Compute forward to requested n if necessary */
    if (n > cache_last) {
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            /* a(i) = 10 * a(i-1) + 31 */
            cache[i] = cache[i - 1] * (seq_t)10 + (seq_t)31;
        }
        cache_last = n;
    }

    return cache[n];
}
