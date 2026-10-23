/*
 * A126497  Number of base 29 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1
 * For n ≥ 1: let dp[d] be the number of length‑k sequences ending with digit d.
 *   initialise dp[d] = 1 for k = 1.
 *   For each next position k → k+1:
 *     next[d] = Σ_{d' = max(0,d-3)}^{min(28,d+3)} dp[d'] .
 *   After processing all positions up to n, a(n) = Σ_{d=0}^{28} dp[d].
 */

seq_t a(idx_t n)
{
    /* When n = 0 the sequence has a single 0‑digit representation. */
    if (n == 0)
        return (seq_t)1;

    /* Local arrays – 29 entries for digits 0 … 28. */
    seq_t dp[29];
    seq_t next[29];

    /* Initialise for strings of length 1. */
    for (idx_t d = 0; d < 29; ++d)
        dp[d] = (seq_t)1;

    /* Build up to length n. */
    for (idx_t pos = 2; pos <= n; ++pos)
    {
        for (idx_t d = 0; d < 29; ++d)
        {
            seq_t sum = (seq_t)0;
            idx_t start = (d >= 3) ? d - 3 : 0;
            idx_t end   = (d + 3 <= 28) ? d + 3 : 28;
            for (idx_t d2 = start; d2 <= end; ++d2)
                sum += dp[d2];
            next[d] = sum;
        }
        /* Copy next → dp for the next iteration. */
        for (idx_t d = 0; d < 29; ++d)
            dp[d] = next[d];
    }

    /* Sum all ending‑digit counts to obtain the total for length n. */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < 29; ++d)
        total += dp[d];

    return total;
}
