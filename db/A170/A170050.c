/*
 * A170050  Number of reduced words of length n in Coxeter group on 41
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^36 = I.
 * https://oeis.org/A170050
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 39*(a(n-1)+...+a(n-35)) - 780*a(n-36), n ≥ 36, with
 *            a(0)=1, a(1)=41, a(2)=1640, a(3)=65600, a(4)=2624000,
 *            a(5)=104960000, a(6)=4198400000, a(7)=167936000000,
 *            a(8)=6717440000000, a(9)=268697600000000,
 *            a(10)=10747904000000000, a(11)=429916160000000000,
 *            a(12)=17196646400000000000, a(13)=68786...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_up_to = 0;
static int initialized = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        initialized = 1;
        /* Compute the first 36 terms explicitly */
        ensure_cache(35, &cache, &cache_cap);
        cache[0] = (seq_t)1;
        for (idx_t k = 1; k < 36; ++k) {
            seq_t sum = (seq_t)2;
            for (idx_t i = 1; i <= k; ++i)
                sum += (seq_t)39 * cache[k - i];
            cache[k] = sum;
        }
        cached_up_to = 35;
    }

    if (n <= cached_up_to)
        return cache[n];

    ensure_cache(n, &cache, &cache_cap);
    for (idx_t k = cached_up_to + 1; k <= n; ++k) {
        if (k < 36) {
            seq_t sum = (seq_t)2;
            for (idx_t i = 1; i <= k; ++i)
                sum += (seq_t)39 * cache[k - i];
            cache[k] = sum;
        } else {
            seq_t sum = (seq_t)0;
            for (idx_t i = 1; i <= 35; ++i)
                sum += (seq_t)39 * cache[k - i];
            sum -= (seq_t)780 * cache[k - 36];
            cache[k] = sum;
        }
    }
    cached_up_to = n;
    return cache[n];
}
