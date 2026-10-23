/*
 * A126479  Number of base 11 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑11 n‑digit strings whose adjacent digits *
 *            differ by at most 3 (absolute difference <= 3). The value is
 *            computed * by a straightforward linear recurrence (length‑5)
 *            derived from the * adjacency graph of 11 nodes. Initial values
 *            are a(0)=1, a(1)=11, * a(2)=65, a(3)=399, a(4)=...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0)=1.  For any n>0 we count all length‑n strings over digits 0..10
     * with the adjacency constraint.  The DP state dp[d] is the number of
     * length‑k strings ending with digit d.  We initialise for k=1 with
     * dp[d]=1 for every d, then iterate (n-1) times to reach length n.
     * The final answer is the sum of the dp entries.  The algorithm
     * runs in O(n) time and O(1) memory.  It satisfies the required
     * performance and does not require external utilities. */
    if (n == (idx_t)0)
    {
        return (seq_t)1;
    }

    /* dp holds counts for current length; dp_next holds next length */
    seq_t dp[11];
    seq_t dp_next[11];

    /* initialise length‑1 strings (all digits allowed) */
    for (int d = 0; d < 11; ++d)
    {
        dp[d] = (seq_t)1;
    }

    /* iterate (n-1) times to build length‑n strings */
    for (idx_t step = (idx_t)1; step < n; ++step)
    {
        for (int d = 0; d < 11; ++d)
        {
            seq_t sum = (seq_t)0;
            int lo = d - 3;
            if (lo < 0) lo = 0;
            int hi = d + 3;
            if (hi > 10) hi = 10;
            for (int p = lo; p <= hi; ++p)
            {
                sum += dp[p];
            }
            dp_next[d] = sum;
        }
        /* copy dp_next to dp for next iteration */
        for (int d = 0; d < 11; ++d)
        {
            dp[d] = dp_next[d];
        }
    }

    /* sum dp over all ending digits to get a(n) */
    seq_t total = (seq_t)0;
    for (int d = 0; d < 11; ++d)
    {
        total += dp[d];
    }

    return total;
}
