/*
 * A126399  Number of base 12 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of length‑n strings over digits 0…11 where the
 *            absolute difference between neighboring digits is ≤2. This is
 *            computed by dynamic programming: DP[1][d] = 1 for all d in 0…11
 *            DP[k+1][d] = Σ DP[k][p] over all p with |p‑d| ≤2 a(n) = Σ
 *            DP[n][d] (n≥1), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0ULL)
        return (seq_t)1;

    /* DP arrays for current and next step. */
    seq_t dp[12];
    seq_t newdp[12];

    /* Initialize DP for length 1: each digit is allowed. */
    for (int d = 0; d < 12; ++d)
        dp[d] = (seq_t)1;

    /* Iterate lengths 2 … n. */
    for (idx_t step = 1; step < n; ++step)
    {
        for (int d = 0; d < 12; ++d)
        {
            seq_t sum = 0;
            int start = d - 2;
            if (start < 0) start = 0;
            int end = d + 2;
            if (end > 11) end = 11;
            for (int p = start; p <= end; ++p)
                sum += dp[p];
            newdp[d] = sum;
        }
        /* Copy newdp into dp for the next iteration. */
        for (int d = 0; d < 12; ++d)
            dp[d] = newdp[d];
    }

    /* Sum all states to obtain the total number of length‑n strings. */
    seq_t total = 0;
    for (int d = 0; d < 12; ++d)
        total += dp[d];
    return total;
}
