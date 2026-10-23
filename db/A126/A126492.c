/*
 * A126492  Number of base 24 n-digit numbers with adjacent digits differing
 *          by three or less.
 * https://oeis.org/A126492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=1, a(1)=24. For n>=2 let dp[d] be the number of (n-1)-digit
 *            sequences ending with digit d (0≤d<24). Then newdp[d] =
 *            Σ_{k:|d-k|≤3} dp[k]. a(n) = Σ_{d} newdp[d]. This dynamic
 *            programming computes the sequence for any n, with no external
 *            tables
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0U) return (seq_t)1;
    if (n == (idx_t)1U) return (seq_t)24;

    seq_t dp[24];
    for (int d = 0; d < 24; ++d) dp[d] = (seq_t)1;

    for (idx_t len = 2; len <= n; ++len) {
        seq_t newdp[24];
        for (int d = 0; d < 24; ++d) {
            seq_t sum = (seq_t)0;
            int start = d - 3;
            if (start < 0) start = 0;
            int end = d + 3;
            if (end > 23) end = 23;
            for (int k = start; k <= end; ++k) sum += dp[k];
            newdp[d] = sum;
        }
        for (int d = 0; d < 24; ++d) dp[d] = newdp[d];
    }

    seq_t total = (seq_t)0;
    for (int d = 0; d < 24; ++d) total += dp[d];
    return total;
}
