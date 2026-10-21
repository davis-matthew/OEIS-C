/*
 * A090251  a(n) = 29a(n-1) - a(n-2), starting with a(0) = 2 and a(1) = 29.
 * https://oeis.org/A090251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 29*a(n-1) - a(n-2), a(0)=2, a(1)=29
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 none,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>


seq_t a(idx_t n)
{
    /* Dynamically grow the cache to cover index n */
    size_t needed = (size_t)n + 1;
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static size_t cached_len = 0;

    if (needed > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : needed;
        if (new_cap < needed) new_cap = needed;

        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;                     /* allocation failed */

        /* Copy existing terms */
        for (size_t i = 0; i < cached_len; ++i) tmp[i] = cache[i];

        /* Replace old array */
        free(cache);
        cache = tmp;
        cache_capacity = new_cap;
    }

    /* Initialise the base cases if not already done */
    if (cached_len == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)29;
        cached_len = 2;
    }
    else if (cached_len == 1) {
        cache[1] = (seq_t)29;
        cached_len = 2;
    }

    /* Compute any missing terms up to n */
    size_t target = (size_t)n;
    if (target >= cached_len) {
        for (size_t i = cached_len; i <= target; ++i) {
            seq_t val = (seq_t)29 * cache[i - 1] - cache[i - 2];
            cache[i] = val;
        }
        cached_len = target + 1;
    }

    return cache[target];
}
