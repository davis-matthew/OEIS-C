/*
 * A219610  Number of ways n can be written as sum of squares < n.
 * https://oeis.org/A219610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 234, u32 n <= 1007, u64 n <= 5179,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A219610
 *
 * Formula: a(n) = number of ways n can be written as a sum of squares
 *           less than n (order of summands does not matter).  Each
 *           summand is of the form k * s^2 with s ≥ 1, k ≥ 1
 *           and s^2 < n.  a(0) = 1 by convention.
 *
 * Implementation: classic coin‑changing dynamic programming
 *   dp[0] = 1
 *   for each square d = s^2, s ≥ 1, d < n
 *       for i = d … n
 *           dp[i] += dp[i – d]
 *
 * Complexity: O(n * sqrt(n)).  Works for all n ≥ 0
 *   (the function may be called in any order; no memoisation
 *   between calls is performed).
 */

#include <stdlib.h>

/* Main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Special case n = 0: one empty representation */
    if (n == 0)
        return (seq_t)1;

    /* Allocate a temporary array of size n+1, zero‑initialised */
    seq_t *dp = (seq_t *)calloc(n + 1, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;                /* allocation failure → safe value */

    dp[0] = (seq_t)1;

    /* Loop over squares d = s^2 with s ≥ 1 and d < n */
    for (idx_t s = 1; ; ++s) {
        idx_t d = s * s;
        if (d >= n)
            break;
        for (idx_t i = d; i <= n; ++i)
            dp[i] += dp[i - d];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
