/*
 * A035983  Number of partitions of n into parts not of the form 21k, 21k+5 or
 *          21k-5. Also number of partitions with at most 4 parts of size 1
 *          and differences between parts at distance 9 are greater than 1.
 * https://oeis.org/A035983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n)=∑_{k≤n, k≢0,5,16 (mod 21)} a(n−k) */

/* Number of partitions of n into parts not congruent to 0,5 or 16 modulo 21. */
#include <stdlib.h> /* malloc, realloc, free */

seq_t a(idx_t n) {
    static seq_t *dp = NULL;       /* memoised DP table */
    static idx_t max_n = 0;        /* largest n that has been computed */

    /* If we already have the value return it. */
    if (n <= max_n) {
        return dp[n];
    }

    /* Allocate or resize the DP array for the new maximum n.  We recompute the
     * whole table from scratch – the cost is about ½·n² additions which is
     * fine for the required limits (n ≈ 10 000). */
    dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!dp) {
        /* Out of memory – abort with a sentinel value. */
        return 0;
    }

    /* Initialise dp[0] = 1, others to 0. */
    for (idx_t i = 0; i <= n; ++i) {
        dp[i] = 0;
    }
    dp[0] = 1;

    /* Build the partition counts using allowed parts. */
    for (idx_t part = 1; part <= n; ++part) {
        /* Skip parts congruent to 0, 5 or 16 modulo 21. */
        if (part % 21 == 0 || part % 21 == 5 || part % 21 == 16) {
            continue;
        }
        for (idx_t s = part; s <= n; ++s) {
            dp[s] += dp[s - part];
        }
    }

    max_n = n;            /* record the new maximum */
    return dp[n];
}
