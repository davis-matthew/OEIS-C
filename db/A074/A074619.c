/*
 * A074619  a(n) = 6^n + 7^n.
 * https://oeis.org/A074619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6ⁿ + 7ⁿ. Recurrence: a(0) = 2, a(1) = 13, a(n) =
 *            13*a(n-1) - 42*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;   /* number of terms stored (cache[0..cached_len-1]) */

    /* If already computed, return the cached value. */
    if (n < cached_len)
        return cache[n];

    /* Allocate initial storage or enlarge it to hold up to n. */
    if (cache == NULL) {
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        cached_len = 0;
    } else if (n + 1 > cached_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    }

    /* Initialise the first two terms if necessary. */
    if (cached_len == 0) {
        cache[0] = (seq_t)2;
        cached_len = 1;
    }
    if (cached_len <= 1 && n >= 1) {
        cache[1] = (seq_t)13;
        cached_len = 2;
    }

    /* Compute the remaining terms up to n. */
    for (idx_t i = cached_len; i <= n; ++i) {
        cache[i] = (seq_t)13 * cache[i - 1] - (seq_t)42 * cache[i - 2];
    }
    cached_len = n + 1;
    return cache[n];
}
