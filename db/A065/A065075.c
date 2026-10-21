/*
 * A065075  Sum of digits of the sum of the preceding numbers.
 * https://oeis.org/A065075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = sum of digits of (a(1)+...+a(n-1))
 * Range:     u8 n <= 30, u16 n <= 3658, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 1, a(2) = 1 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t sum = 0;

    if (n == 0) return 0; /* not used; safeguard */

    /* Initialise the cache if necessary */
    if (cache == NULL) {
        /* At least two entries to support a(2) */
        cap = (n > 2) ? n : 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        cache[0] = 1;
        if (cap > 1) cache[1] = 1;
        sum = (cap > 1) ? (seq_t)2 : (seq_t)1;
        len = (cap > 1) ? 2 : 1;
    }

    /* Return cached values directly if available */
    if (n <= len) {
        return cache[n - 1];
    }

    /* Grow the cache to accommodate new terms */
    if (cap < n) {
        size_t new_cap = cap * 2;
        if (new_cap < n) new_cap = n;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        for (size_t i = 0; i < len; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
        cap = new_cap;
    }

    /* Compute terms from len up to n-1 */
    for (size_t i = len; i < n; ++i) {
        seq_t val = digit_sum(sum);
        cache[i] = val;
        sum += val;
    }
    len = n;
    return cache[n - 1];
}
