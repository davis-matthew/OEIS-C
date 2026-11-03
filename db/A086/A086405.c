/*
 * A086405  Row T(n,3) of number array A086404.
 * https://oeis.org/A086405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*(a(n-1) - a(n-2)), a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 998 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* Convert the 64‑bit index to a size_t for array access. */
    size_t idx = (size_t)n;

    /* Ensure at least the first two values are cached. */
    if (cache_size < 2) {
        cache_size = 2;
        cache = mem_new(seq_t, cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)4;
    }

    /* Return immediately if the value has already been computed. */
    if (idx < cache_size) {
        return cache[idx];
    }

    /* Grow the cache to contain the requested index. */
    size_t new_size = idx + 1;
    cache = mem_resize(seq_t, cache, new_size);

    /* Compute new values using the recurrence. */
    for (size_t i = cache_size; i < new_size; ++i) {
        seq_t diff = cache[i - 1] - cache[i - 2];
        cache[i] = (seq_t)((seq_t)6 * diff);
    }

    cache_size = new_size;
    return cache[idx];
}
