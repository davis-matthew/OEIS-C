/*
 * A045776  a(n+1) is smallest multiple of (sum of digits of a(n)) which is >
 *          a(n).
 * https://oeis.org/A045776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3824, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A045776
   a(0) = 1
   a(n+1) = the smallest multiple of sum_of_digits(a(n)) that is > a(n)   */

/* Note: uses digit_sum from utils/digit_sum.h */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

/* External linkage */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* Grow cache to accommodate index n */
    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – return 0 as a fallback (unlikely) */
            return 0;
        }
        if (!cache) {
            /* First allocation: set first term */
            new_cache[0] = 1;
            cache_cap = 1;
        }
        cache = new_cache;

        /* Compute missing terms */
        for (size_t i = cache_cap; i <= n; ++i) {
            seq_t cur = cache[i - 1];
            seq_t s   = digit_sum(cur);          /* sum of digits of a(i-1) */
            /* smallest multiple of s that is > cur */
            seq_t next = (cur / s + 1) * s;
            cache[i] = next;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
