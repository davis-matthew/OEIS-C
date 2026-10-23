/*
 * A145865  a(0) = 0, a(1) = 1, a(2n) = a(n), a(2n+1) = a(n) - a(n+1).
 * https://oeis.org/A145865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 0, a(1) = 1 * a(2n) = a(n) * a(2n+1) = a(n) - a(n+1)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special cases for n = 0, 1 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache can hold all indices up to n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;      /* out‑of‑memory: return 0 (fallback) */
        cache = tmp;
        cache_size = new_size;
    }

    /* Initialise first two terms */
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)1;

    /* Compute values iteratively using the recurrence */
    for (idx_t i = 2; i <= n; ++i) {
        if ((i & 1) == 0) {
            cache[i] = cache[i >> 1];
        } else {
            cache[i] = cache[(i >> 1)] - cache[(i >> 1) + 1];
        }
    }

    return cache[n];
}
