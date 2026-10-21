/*
 * A067326  Fifth column of triangle A067323.
 * https://oeis.org/A067326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Catalan(n+5) - ∑_{k=0}^{3} Catalan(k)*Catalan(n+4-k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes Catalan numbers up to the required
   index using the recurrence
     C(0) = 1,
     C(i) = C(i-1) * (4*i - 2) / (i + 1)   for i ≥ 1
   The cached values are reused across calls, so the function runs in
   essentially O(1) time per query after an initial O(m) precomputation
   where m is the maximum n requested.  It handles any n ≥ 0. */

#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t cat_cache[20001] = {0};
    static size_t cat_cap = 0;

    /* Determine how many Catalan numbers we need (up to n+5). */
    size_t needed = (size_t)n + 5 + 1;   /* indices 0 .. n+5 inclusive */

    if (needed > cat_cap) {
        /* Ensure the first Catalan number is initialized */
        if (cat_cap == 0) {
            cat_cache[0] = 1;
            cat_cap = 1;
        }
        /* Compute missing Catalan numbers using the recurrence */
        for (size_t i = cat_cap; i < needed; ++i) {
            if (i == 0) {
                cat_cache[0] = 1;
            } else {
                /* C(i) = C(i-1) * (4*i - 2) / (i + 1) */
                cat_cache[i] = cat_cache[i - 1] *
                               (seq_t)(4 * i - 2) /
                               (seq_t)(i + 1);
            }
        }
        cat_cap = needed;
    }

    /* Retrieve required Catalan numbers from cache */
    seq_t res = cat_cache[n + 5];
    res -= cat_cache[0] * cat_cache[n + 4];
    res -= cat_cache[1] * cat_cache[n + 3];
    res -= cat_cache[2] * cat_cache[n + 2];
    res -= cat_cache[3] * cat_cache[n + 1];

    return res;
}
