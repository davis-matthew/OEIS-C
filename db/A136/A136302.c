/*
 * A136302  Transform of A000027 by the T_{1,1} transformation (see link).
 * https://oeis.org/A136302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 3*a(n+2) - 2*a(n+1) + a(n), initial values: a(1)=2,
 *            a(2)=6, a(3)=15 (offset 1)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* memoized values a(1)..a(cache_size) */
    static idx_t cache_size = 0;

    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    /* Extend cache if needed */
    if (n > cache_size) {
        seq_t *newcache = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!newcache) return 0;   /* allocation failure – unlikely at requested scales */

        cache = newcache;

        /* Ensure the first three values are set (idempotent). */
        if (n >= 1) cache[0] = 2;
        if (n >= 2) cache[1] = 6;
        if (n >= 3) cache[2] = 15;

        /* Compute missing terms using the recurrence. */
        for (idx_t i = 4; i <= n; ++i) {
            cache[i-1] = cache[i-2] * 3 - cache[i-3] * 2 + cache[i-4];
        }

        cache_size = n;
    }

    return cache[n-1];
}
