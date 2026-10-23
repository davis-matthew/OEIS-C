/*
 * A022388  Fibonacci sequence beginning 6, 13.
 * https://oeis.org/A022388
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 6, a(1) = 13
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the two base cases directly */
    if (n == 0u) return (seq_t)6;
    if (n == 1u) return (seq_t)13;

    /* Static cache of previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cap   = 0u;   /* number of entries currently stored      */

    /* Lazy initialization: create space for the first two terms. */
    if (cap == 0u) {
        cache = (seq_t *)malloc(2u * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* unlikely, but defensive  */
        cache[0] = (seq_t)6;
        cache[1] = (seq_t)13;
        cap = 2u;
    }

    /* Grow the cache up to the requested index if necessary. */
    if (n >= cap) {
        idx_t old_cap = cap;

        /* Determine a new capacity: double until it exceeds n. */
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1u;

        /* Ensure the new capacity is at least n+1. */
        if (new_cap <= n + 1u) new_cap = n + 1u;

        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        cache = tmp;
        cap   = new_cap;

        /* Compute the new terms using the Fibonacci‑like recurrence. */
        for (idx_t i = old_cap; i < cap; ++i) {
            /* For i == 1 the recurrence is not used; we start at i == 2. */
            if (i < 2u) continue;
            cache[i] = cache[i-1] + cache[i-2];
        }
    }

    return cache[n];
}
