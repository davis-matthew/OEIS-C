/*
 * A231503  a(n) = Sum_{i=0..n} digsum_3(i)^2, where digsum_3(i) = A053735(i).
 * https://oeis.org/A231503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} (digsum_3(i))^2, where digsum_3(i) is the
 *            sum of base‑3 digits of i
 * Range:     u8 n <= 24, u16 n <= 1436, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cumulative sums up to current size-1 */
static idx_t cache_cap = 0;          /* number of elements currently stored */

/* helper: sum of base‑3 digits of x */
static inline unsigned int digit_sum_base3(idx_t x) {
    unsigned int sum = 0;
    while (x > 0) {
        sum += (unsigned int)(x % 3U);
        x /= 3U;
    }
    return sum;
}

seq_t a(idx_t n) {
    if (cache_cap == 0) {
        /* initialise with a(0) = 0 */
        cache = (seq_t *)malloc(1 * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* fallback on allocation failure */
        cache[0] = (seq_t)0;
        cache_cap = 1;
    }

    /* extend cache up to n if needed */
    if (n >= cache_cap) {
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure */
        cache = new_cache;

        seq_t *ptr = cache + cache_cap;
        idx_t i;
        for (i = cache_cap; i <= n; ++i) {
            unsigned int dsum = digit_sum_base3(i);
            seq_t square = (seq_t)dsum * (seq_t)dsum;
            ptr[0] = cache[i - 1] + square;
            ++ptr;
        }
        cache_cap = n + 1;
    }

    return cache[n];
}
