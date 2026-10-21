/*
 * A057744  Expansion of (1-2*x^3)/(1-2*x-x^3+2*x^4).
 * https://oeis.org/A057744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=7, a(n)=2*a(n-1)+a(n-3)-2*a(n-4)
 *            for n>3
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoised results */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;
static size_t cache_len = 0;

/* Ensure the cache has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;
    size_t new_cap = (cache_capacity == 0) ? 8 : cache_capacity * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;
    cache_capacity = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise the first four values on first call. */
    if (cache_len == 0) {
        ensure_capacity(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)7;
        cache_len = 4;
    }

    size_t target = (size_t)n;

    /* If the requested index is beyond what we've already computed,
       extend the cache and fill in missing values. */
    if (target >= cache_len) {
        ensure_capacity(target + 1);
        for (size_t i = cache_len; i <= target; ++i) {
            seq_t val;
            if (i < 4) {
                /* Redundant but harmless – keeps the code isolated. */
                switch (i) {
                    case 0: val = (seq_t)1; break;
                    case 1: val = (seq_t)2; break;
                    case 2: val = (seq_t)4; break;
                    case 3: val = (seq_t)7; break;
                }
            } else {
                /* Recurrence: a(i) = 2*a(i-1) + a(i-3) - 2*a(i-4) */
                val = (cache[i - 1] << 1) + cache[i - 3] - (cache[i - 4] << 1);
            }
            cache[i] = val;
        }
        cache_len = target + 1;
    }

    return cache[target];
}
