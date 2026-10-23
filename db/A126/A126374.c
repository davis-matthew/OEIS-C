/*
 * A126374  Number of base 20 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126374
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base-20 n‑digit strings with adjacent digits
 *            differing by at most 1. Recurrence: a(0) = 1. For n ≥ 1 compute
 *            by dynamic programming: dp[d] = number of ways to form a string
 *            of current length ending with digit d
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* dp[d] holds counts for strings of current length ending in digit d */
    seq_t dp[20];
    seq_t next[20];
    seq_t total = 0;

    /* length 1: each digit appears once */
    for (idx_t d = 0; d < 20; ++d)
        dp[d] = 1;
    total = 20;                /* a(1) */

    for (idx_t step = 2; step <= n; ++step) {
        total = 0;
        for (idx_t d = 0; d < 20; ++d) {
            seq_t val = dp[d];
            if (d > 0)     val += dp[d - 1];
            if (d < 19)    val += dp[d + 1];
            next[d] = val;
            total += val;
        }
        for (idx_t d = 0; d < 20; ++d)
            dp[d] = next[d];
    }

    return total;
}
