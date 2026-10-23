/*
 * A124610  a(n) = 5*a(n-1) + 2*a(n-2), n > 1; a(0) = a(1) = 1.
 * https://oeis.org/A124610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2); a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cache_filled = 0;

    /* Initialize the first two terms */
    if (cache_filled == 0) {
        ensure_capacity(2, &cache_capacity, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_filled = 2;
    }

    /* Target index as a size_t for array bounds */
    size_t target = (size_t)n;

    /* Ensure we have enough allocated space */
    if (target + 1 > cache_capacity) {
        ensure_capacity(target + 1, &cache_capacity, (void **)&cache, sizeof(seq_t));
    }

    /* Compute any missing terms up to the target */
    if (target + 1 > cache_filled) {
        for (size_t i = cache_filled; i <= target; ++i) {
            cache[i] = (seq_t)5 * cache[i - 1] + (seq_t)2 * cache[i - 2];
        }
        cache_filled = target + 1;
    }

    return cache[target];
}
