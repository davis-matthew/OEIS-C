/*
 * A029151  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^6)*(1-x^8)).
 * https://oeis.org/A029151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 6 and 8.
 *            Recurrence: a(0)=1, a(n)=∑ a(n‑p) for each p∈{2,3,6,8} with n≥p
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The DP is recomputed from scratch each time the requested index n
   exceeds the previous maximum.  This method guarantees that the
   order of parts does not influence the count. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static const uint8_t parts[4] = {2, 3, 6, 8};

    size_t required = (size_t)n + 1;
    if (required <= cache_size)            // already computed
        return cache[(size_t)n];

    /* Allocate new cache of required size.  Initialize all entries to 0. */
    seq_t *new_cache = (seq_t *)malloc(required * sizeof(seq_t));
    if (!new_cache)                      /* allocation failed */
        return (seq_t)0;

    for (size_t i = 0; i < required; ++i)
        new_cache[i] = (seq_t)0;
    new_cache[0] = (seq_t)1;              /* base case */

    /* Classic coin–change DP: iterate over each part as outer loop. */
    for (int k = 0; k < 4; ++k) {
        size_t coin = (size_t)parts[k];
        if (coin >= required)            /* coin larger than n, skip */
            continue;
        for (size_t i = coin; i < required; ++i) {
            new_cache[i] += new_cache[i - coin];
        }
    }

    /* Replace old cache. */
    free(cache);
    cache      = new_cache;
    cache_size = required;

    return cache[(size_t)n];
}
