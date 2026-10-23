/*
 * A176290  Hankel transform of A105872.
 * https://oeis.org/A176290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 26, i128 n <= 54,
 *            big any n
 * Helpers:   grow_to_at_least
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 10*a(n-1) - 27*a(n-2) + 10*a(n-3) - a(n-4)
   with initial terms a(0)=1, a(1)=2, a(2)=-3, a(3)=-75.   */

#include <stdlib.h>
#include "../utils/grow_to_at_least.h"

/* Return the n-th term of the Hankel transform of A105872. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0; /* number of valid terms in cache */

    /* Initialize cache with the first four known terms. */
    if (cache == NULL) {
        grow_to_at_least(&cache, &cache_cap, 4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)-3;
        cache[3] = (seq_t)-75;
        cache_len = 4;
    }

    if ((size_t)n < cache_len) {
        return cache[n];
    }

    /* Extend the cache if necessary. */
    grow_to_at_least(&cache, &cache_cap, (size_t)n + 1);

    /* Compute missing terms using the recurrence. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t next = (seq_t)10 * cache[i - 1]
                   - (seq_t)27 * cache[i - 2]
                   + (seq_t)10 * cache[i - 3]
                   - cache[i - 4];
        cache[i] = next;
    }
    cache_len = (size_t)n + 1;
    return cache[n];
}
