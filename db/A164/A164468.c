/*
 * A164468  Number of binary strings of length n with no substrings equal to
 *          0001, 0100, or 1110.
 * https://oeis.org/A164468
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + 2 for n > 6; initial values: a(4)=13,
 *            a(5)=20, a(6)=30
 * Range:     u8 n <= 11, u16 n <= 25, u32 n <= 54, u64 n <= 112,
 *            u128 n <= 229, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy cache of previously computed values.
       cache[0..max] contains a(i) for all 0 <= i <= max.
       We only need to store from 0 to n, but we initialise
       the array at size 7 to hold the initial values. */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of slots currently allocated */

    /* Sequence has offset 4; behaviour for n < 4 is irrelevant. */
    if (n < 4)
        return 0;   /* unspecified for indices below the offset */

    /* Initialise the cache with the first known values. */
    if (!cache) {
        cache_size = 7;                     /* indices 0..6 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – return 0 */
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = 0;
        cache[4] = (seq_t)13;
        cache[5] = (seq_t)20;
        cache[6] = (seq_t)30;
    }

    /* Grow the cache if the requested index is beyond the current size. */
    if ((size_t)n >= cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;           /* allocation failure – return 0 */
        /* Initialise any new slots to 0. */
        for (size_t i = cache_size; i < new_size; ++i)
            new_cache[i] = 0;
        /* Compute all new values using the recurrence. */
        for (size_t i = cache_size; i <= (size_t)n; ++i) {
            if (i <= 6) continue;               /* already initialised */
            new_cache[i] = new_cache[i-1] + new_cache[i-3] + (seq_t)2;
        }
        cache = new_cache;
        cache_size = new_size;
    }
    return cache[n];
}
