/*
 * A047899  a(1) = 7; for n > 0, a(n+1) = a(n) * sum of digits of a(n).
 * https://oeis.org/A047899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 7; a(n+1) = a(n) * digit_sum(a(n))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 23,
 *            big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    if (n < 1)
        return 0; /* undefined for n < 1, but return 0 */

    /* Initialize first term if needed */
    if (cached_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0; /* allocation failure */
        cache[0] = (seq_t)7;      /* a(1) */
        cached_len = 1;
    }

    /* Return cached value if already computed */
    if (n <= cached_len)
        return cache[n - 1];

    /* Compute missing terms up to n */
    for (idx_t i = cached_len; i < n; ++i) {
        seq_t prev = cache[i - 1];
        seq_t sum = digit_sum(prev);
        seq_t next = prev * sum;

        /* Grow cache */
        seq_t *new_cache = (seq_t *)realloc(cache, (i + 1) * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        cache = new_cache;
        cache[i] = next;
    }
    cached_len = n;

    return cache[n - 1];
}
