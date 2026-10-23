/*
 * A122439  Expansion of 1/(1-2x-x^2+4x^4).
 * https://oeis.org/A122439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(3)=12 * Recurrence for n≥4:
 *            a(n)=2*a(n-1)+a(n-2)-4*a(n-4)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed_up_to = 0;   /* next index that needs to be computed */

seq_t a(idx_t n)
{
    /* Make sure the cache holds indices up to n */
    ensure_cache(n, &cache, &cache_cap);

    /* Compute missing terms up to n */
    while (computed_up_to <= n) {
        idx_t i = computed_up_to;
        if (i == 0) {
            cache[i] = (seq_t)1;
        } else if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)5;
        } else if (i == 3) {
            cache[i] = (seq_t)12;
        } else {
            seq_t term = 2 * cache[i-1] + cache[i-2] - 4 * cache[i-4];
            cache[i] = term;
        }
        ++computed_up_to;
    }

    return cache[n];
}
