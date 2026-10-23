/*
 * A159626  Positive numbers y such that y^2 is of the form x^2+(x+577)^2 with
 *          integer x.
 * https://oeis.org/A159626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-3)-a(n-6) for n > 6 Initial values: a(1)=545,
 *            a(2)=577, a(3)=613, a(4)=2657, a(5)=2885, a(6)=3133
 * Range:     u8 none, u16 n <= 9, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; behaviour undefined below 1. */
    if (n < 1) {
        return 0;
    }

    /* static cache of computed terms; index 0 is unused */
    static seq_t *cache = NULL;
    static size_t cap = 0;            /* number of allocated slots (max index + 1) */

    /* Ensure the cache holds at least the first 6 terms */
    if (cap == 0) {
        cap = 7;                      /* slots 0..6 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        if (!cache) {
            return 0;                 /* allocation failure */
        }
        cache[1] = (seq_t)545;
        cache[2] = (seq_t)577;
        cache[3] = (seq_t)613;
        cache[4] = (seq_t)2657;
        cache[5] = (seq_t)2885;
        cache[6] = (seq_t)3133;
    }

    /* If the requested index is within the already computed block, return it. */
    if (n < cap) {
        return cache[(size_t)n];
    }

    /* Grow the cache to accommodate index n */
    if ((size_t)n >= cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            return 0;                 /* allocation failure */
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all missing terms up to n using the recurrence */
    for (size_t i = 7; i <= (size_t)n; ++i) {
        /*
           a(i) = 6 * a(i-3) - a(i-6)
           All operations are on seq_t; any intermediate overflow
           is handled by the arbitrary‑precision implementation.
        */
        cache[i] = 6 * cache[i - 3] - cache[i - 6];
    }

    return cache[(size_t)n];
}
