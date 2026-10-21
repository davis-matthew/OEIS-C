/*
 * A075543  a(n) = a(n-1) + digit sum(n + 2) with a(0) = 2.
 * https://oeis.org/A075543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n‑1) + digit sum(n+2), a(0) = 2
 * Range:     u8 n <= 40, u16 n <= 4384, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t digit_sum_uint64(idx_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t max_computed = 0;
    static size_t capacity = 0;

    /* Initialize cache if first call. */
    if (cache == NULL) {
        capacity = 16;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        cache[0] = (seq_t)2;
        max_computed = 0;
    }

    if (n > max_computed) {
        /* Ensure enough space for n+1 entries. */
        size_t needed = (size_t)(n + 1);
        if (needed > capacity) {
            capacity = needed;
            cache = (seq_t *)realloc(cache, capacity * sizeof(seq_t));
        }

        /* Compute missing terms. */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t dsum = digit_sum_uint64(i + 2);
            cache[i] = cache[i - 1] + dsum;
        }
        max_computed = n;
    }

    return cache[n];
}
