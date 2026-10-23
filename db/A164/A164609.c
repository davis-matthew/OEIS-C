/*
 * A164609  a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 13.
 * https://oeis.org/A164609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2) for n > 1; a(0) = 1, a(1) = 13
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t last   = 1;        /* largest index already computed */

    /* Ensure there is enough space for index n */
    ensure_cache(n, &cache, &cap);

    /* Initialise base values once */
    if (cache[0] == 0) cache[0] = (seq_t)1;
    if (cache[1] == 0) cache[1] = (seq_t)13;

    if (n < 2) return cache[n];

    /* Compute missing terms */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            /* All operations are performed in seq_t; this is safe
               because we only require exactness when the result fits */
            cache[i] = cache[i-1] * (seq_t)10 - cache[i-2] * (seq_t)17;
        }
        last = n;
    }
    return cache[n];
}
