/*
 * A032303  "EFK" (unordered, size, unlabeled) transform of 2,1,1,1,...
 * https://oeis.org/A032303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p_distinct_ge2(n) + 2 * p_distinct_ge2(n-1) where
 *            p_distinct_ge2(n) is the number of partitions of n into distinct
 *            parts all >= 2. Generating function: (1 + 2*x) * ∏_{k=2..∞} (1 +
 *            x^k)
 * Range:     u8 n <= 26, u16 n <= 74, u32 n <= 230, u64 n <= 778,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of the sequence A032303.  Indexing starts at 0. */
seq_t a(idx_t n)
{
    /* dp[s] holds p_distinct_ge2(s) for all s ≤ current_max */
    static seq_t *dp           = NULL;
    static idx_t current_max   = 0;   /* largest n for which dp has been computed */

    /* Ensure we have a dp array that can hold indices up to n */
    if (n + 1 > current_max) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t*)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) return (seq_t)0;        /* unlikely, defensive */
        dp = new_dp;

        /* Initialise all entries to 0 */
        for (idx_t i = 0; i < new_size; ++i) dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;                    /* empty partition */

        /* Classic distinct‑partitions DP for parts ≥ 2 */
        for (idx_t part = 2; part <= n; ++part) {
            for (idx_t s = n; s >= part; --s)
                dp[s] += dp[s - part];
        }
        current_max = n;
    }

    /* Compute a(n) using the precomputed dp values */
    if (n == 0) return dp[0];  /* a(0) = 1 */

    seq_t result = dp[n];
    result += dp[n - 1] * ((seq_t)2);
    return result;
}
