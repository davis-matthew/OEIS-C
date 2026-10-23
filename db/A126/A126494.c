/*
 * A126494  Number of base 26 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126494
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

/* A126494:  a(0)=1, a(1)=26.
   For n≥2
     dp[1][d] = 1   for each digit d in 0…25
     dp[l+1][j] = Σ dp[l][i]  over all i with |i−j|≤3
   a(n) = Σ dp[n][d] over d=0…25   (sum of all length‑n strings). */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* DP arrays – only 26 entries each */
    seq_t dp[26];
    seq_t ndp[26];

    /* initialise for length = 1 */
    for (idx_t d = 0; d < 26; ++d)
        dp[d] = (seq_t)1;

    /* iterate for lengths 2 … n */
    for (idx_t len = 2; len <= n; ++len)
    {
        for (idx_t j = 0; j < 26; ++j)
        {
            ndp[j] = (seq_t)0;
            idx_t start = (j >= 3) ? (j - 3) : 0;
            idx_t end   = (j <= 22) ? (j + 3) : 25; /* 25 − j ≤ 3 ⇔ j ≥ 22 */
            for (idx_t k = start; k <= end; ++k)
                ndp[j] += dp[k];
        }
        /* swap dp and ndp for next iteration */
        for (idx_t d = 0; d < 26; ++d)
            dp[d] = ndp[d];
    }

    /* sum over all ending digits to get the total count */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < 26; ++d)
        total += dp[d];

    return total;
}
