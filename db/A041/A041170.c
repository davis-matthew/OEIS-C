/*
 * A041170  Numerators of continued fraction convergents to sqrt(95).
 * https://oeis.org/A041170
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 78*a(n-4) - a(n-8), with a(0)=9, a(1)=10, a(2)=29,
 *            a(3)=39, a(4)=731, a(5)=770, a(6)=2271, a(7)=3041
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initial seed values for indices 0..7. */
    static const seq_t base[8] = {9, 10, 29, 39, 731, 770, 2271, 3041};

    /* Allocate and initialise the cache on the first call. */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0; /* OOM protection; caller is unlikely to check. */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = base[i];
    }

    /* If the requested index is already in the cache, return it. */
    if ((idx_t)cache_len > n)
        return cache[(size_t)n];

    /* Grow the cache to hold indices up to n. */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
    if (!new_cache) return 0; /* OOM protection. */
    cache = new_cache;
    cache_len = new_len;

    /* Fill in new entries using the recurrence. */
    for (size_t i = 8; i <= (size_t)n; ++i) {
        seq_t t = mul_small(cache[i - 4], 78);
        cache[i] = t - cache[i - 8];
    }

    return cache[(size_t)n];
}
