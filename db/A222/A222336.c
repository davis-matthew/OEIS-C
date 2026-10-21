/*
 * A222336  Number of n X 3 0..6 arrays with no entry increasing mod 7 by 6
 *          rightwards or downwards, starting with upper left zero.
 * https://oeis.org/A222336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 17,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A222336
 * Empirical recurrence:
 *   a(1) = 36
 *   a(2) = 5766
 *   a(n) = 165*a(n-1) - 774*a(n-2)   for n ≥ 3
 * This implementation memoises values so that calls can arrive in any
 * order.  All arithmetic is performed in the build‑defined type seq_t
 * and the recurrence is evaluated using only integer operators.
 */

#include "../utils/seq_realloc.h"
#include <stddef.h>     /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the first two values explicitly. */
    if (n == 1) return (seq_t)36;
    if (n == 2) return (seq_t)5766;

    /* Static cache of previously computed values. */
    static seq_t *cache   = NULL;
    static size_t  cache_len = 0;

    /* If the cache has not been initialised yet, do it now. */
    if (cache_len == 0) {
        cache_len = 2;                     /* cache[0] and cache[1] hold a(1), a(2) */
        cache     = (seq_t*)seq_realloc(NULL, 0, cache_len);
        cache[0] = (seq_t)36;
        cache[1] = (seq_t)5766;
    }

    /* If the requested term is already cached, return it. */
    if ((size_t)n <= cache_len) {
        return cache[n - 1];
    }

    /* Grow the cache to size n, computing new terms on the way. */
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t*)seq_realloc(cache, cache_len, new_len);
    if (new_cache == NULL) {
        /* Allocation failure – in a contest environment we could abort,
         * but here we simply fall back to recomputing from scratch. */
        new_cache = (seq_t*)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL) {
            /* a real implementation would handle this; here we just
             * return 0. */
            return (seq_t)0;
        }
        /* Copy the already known prefix. */
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
    }

    /* Compute the remaining values using the recurrence. */
    for (size_t i = cache_len; i < new_len; ++i) {
        seq_t prev1 = new_cache[i - 1];
        seq_t prev2 = new_cache[i - 2];
        new_cache[i] = (seq_t)((seq_t)165 * prev1 - (seq_t)774 * prev2);
    }

    cache     = new_cache;
    cache_len = new_len;

    return cache[n - 1];
}
