/*
 * A161584  The list of the k values in the common solutions to the 2
 *          equations 13*k+1 = A^2, 17*k+1 = B^2.
 * https://oeis.org/A161584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+3) = 224*(a(n+2) - a(n+1)) + a(n), a(1)=0, a(2)=15,
 *            a(3)=3360
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* lazy initialisation of first three terms */
    if (cache_len == 0) {
        ensure_capacity(3, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 15;
        cache[2] = 3360;
        cache_len = 3;
    }

    /* extend cache up to requested n */
    if ((idx_t)n > cache_len) {
        ensure_capacity((size_t)n, &cache_cap, (void **)&cache, sizeof(seq_t));
        for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
            size_t idx = i - 1;              /* zero‑based index for a(i) */
            seq_t diff = cache[idx - 1] - cache[idx - 2];
            cache[idx] = 224 * diff + cache[idx - 3];
        }
        cache_len = n;
    }

    return cache[n - 1];
}
