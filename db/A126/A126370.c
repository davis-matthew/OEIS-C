/*
 * A126370  Number of base 16 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=16.
   For n≥2 let dp[d] (d=0…15) be the number of length‑n strings
   ending with digit d.  Then for each d
       dp_next[d] = dp[d] + (d>0?dp[d-1]:0) + (d<15?dp[d+1]:0).
   The answer a(n) is the sum of all dp[d]. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)16;

    seq_t curr[16];
    seq_t next[16];

    for (int d = 0; d < 16; ++d) curr[d] = (seq_t)1;

    for (idx_t len = 2; len <= n; ++len)
    {
        for (int d = 0; d < 16; ++d)
        {
            seq_t sum = curr[d];
            if (d > 0)   sum += curr[d - 1];
            if (d < 15)  sum += curr[d + 1];
            next[d] = sum;
        }
        for (int d = 0; d < 16; ++d) curr[d] = next[d];
    }

    seq_t total = 0;
    for (int d = 0; d < 16; ++d) total += curr[d];
    return total;
}
