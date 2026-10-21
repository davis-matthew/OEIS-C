/*
 * A078366  A Chebyshev S-sequence with Diophantine property.
 * https://oeis.org/A078366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 17*a(n-1) - a(n-2), a(0) = 1, a(1) = 17
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow cache if needed */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)17;
            } else {
                /* Use only seq_t arithmetic; works for built‑in and arbitrary‑precision types */
                cache[i] = cache[i-1] * (seq_t)17 - cache[i-2];
            }
        }
        cache_size = new_size;
    }

    return cache[n];
}
