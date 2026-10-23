/*
 * A187511  Number of 6-step S, E, and NW-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) for n > 7 Initial values:
 *            a(1) = 0, a(2) = 0, a(3) = 36, a(4) = 272, a(5) = 772, a(6) =
 *            1525, a(7) = 2524 (a(8) onward follow the recurrence)
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation – stored 1‑based values as 0‑based array */
    static seq_t *cache   = NULL;
    static idx_t cache_sz = 0;   /* number of terms currently stored */

    if (n == 0)               /* undefined below offset, harmless default */
        return 0;

    /* Already calculated? */
    if (n <= cache_sz)
        return cache[n - 1];

    /* Expand the cache to hold terms up to n */
    idx_t needed = n;
    seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!tmp)                /* allocation failure – terminate */
        abort();
    cache = tmp;

    /* Initial values for the first seven terms */
    static const seq_t init[7] = {
        0, 0, 36, 272, 772, 1525, 2524
    };

    /* Compute missing terms */
    for (idx_t i = cache_sz + 1; i <= needed; ++i) {
        seq_t val;
        if (i <= 7) {
            val = init[i - 1];
        } else {
            /* a(i) = 3*a(i-1) - 3*a(i-2) + a(i-3) */
            val = cache[i - 2] * 3;
            val -= cache[i - 3] * 3;
            val += cache[i - 4];
        }
        cache[i - 1] = val;
    }

    cache_sz = needed;
    return cache[n - 1];
}
