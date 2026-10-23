/*
 * A027357  Number of partitions of n into distinct odd parts, the greatest
 *          being congruent to 1 mod 4.
 * https://oeis.org/A027357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct odd parts, the
 *            greatest being congruent to 1 (mod 4). (OEIS A027357)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* dp[s] = number of partitions of s into distinct odd parts, no restriction
   on the largest part.  ans counts only those whose largest part ≡ 1 (mod 4). */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; other values are irrelevant. */
    if (n < 1)
        return 0;

    /* Allocate an array of n + 1 entries.  */
    size_t cap = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!dp)
        return 0;                     /* unlikely in the test harness */

    /* Initialise to zero and set dp[0] = 1 (empty partition). */
    for (idx_t i = 0; i < cap; ++i)
        dp[i] = 0;
    dp[0] = 1;

    seq_t ans = 0;

    /* Process odd parts in increasing order.  */
    for (idx_t k = 1; k <= n; k += 2)
    {
        seq_t old = dp[n];          /* value of dp[n] before adding part k */

        /* Update dp in reverse to keep each part distinct.  */
        for (idx_t s = n; s >= k; --s)
            dp[s] += dp[s - k];

        seq_t newv = dp[n];         /* value after adding part k */

        /* If k ≡ 1 (mod 4), partitions where k is the largest part were
           added exactly by newv - old.  */
        if ((k & 3) == 1)
            ans += newv - old;
    }

    free(dp);
    return ans;
}
