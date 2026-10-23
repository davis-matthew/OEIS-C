/*
 * A126371  Number of base 17 n-digit numbers with adjacent digits differing
 *          by one or less.
 * https://oeis.org/A126371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of base‑17 n‑digit sequences where each pair of
 * consecutive digits differ by at most 1 (leading zeros allowed).
 * Recurrence:
 *   dp[1][d] = 1              for all 0 ≤ d ≤ 16
 *   dp[k][d] = dp[k-1][d] + dp[k-1][d-1] + dp[k-1][d+1]   (within bounds)
 *   a(n) = Σ_{d=0}^{16} dp[n][d]
 * Initial values: a(0)=1, a(1)=17
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* dp holds counts for current length, nextdp for next length */
    seq_t dp[17], nextdp[17];
    for (idx_t d = 0; d < 17; ++d)
        dp[d] = (seq_t)1;            /* length 1 */

    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t d = 0; d < 17; ++d) {
            nextdp[d] = dp[d];                     /* same digit */
            if (d > 0)        nextdp[d] += dp[d-1]; /* digit-1 */
            if (d + 1 < 17)   nextdp[d] += dp[d+1]; /* digit+1 */
        }
        /* copy back to dp for next iteration */
        for (idx_t d = 0; d < 17; ++d)
            dp[d] = nextdp[d];
    }

    /* sum over all ending digits to get a(n) */
    seq_t total = (seq_t)0;
    for (idx_t d = 0; d < 17; ++d)
        total += dp[d];

    return total;
}
