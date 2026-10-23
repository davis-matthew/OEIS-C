/*
 * A026820  Euler's table: triangular array T read by rows, where T(n,k) =
 *          number of partitions in which every part is <= k for 1 <= k <= n.
 *          Also number of partitions of n into at most k parts.
 * https://oeis.org/A026820
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, changed
 * Formula:   a(n) = T(r,c) * where T(n,k) = number of partitions of n into
 *            parts ≤ k * (equivalently the number of partitions of n into at
 *            most k parts). * For a given global index n (offset 1) find the
 *            row r that satisfies * 1 + 2 + … + r−1 < n ≤ 1 + 2 + … + r. *
 *            Then set c = n − (1 + 2 + … + r−1) (1‑based c...
 * Range:     u8 n <= 145, u16 n <= 963, u32 n <= 8180, u64 n <= 9025,
 *            u128 n <= 9025, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool, not actually used but safe for all builds */

static seq_t dp[201][201];   /* 201 is > sqrt(2*10000) ≈ 142 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* off the defined offset */

    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 0;
    idx_t sum = 0;
    while (sum < n) { r++; sum += r; }

    idx_t prev_sum = sum - r;      /* sum_{i=1}^{r-1} i */
    idx_t c = n - prev_sum;        /* column index (1‑based) */

    /* Initialise base row 0 */
    for (idx_t k = 0; k <= c; ++k) dp[0][k] = 1;
    dp[0][0] = 1;                  /* T(0,0)=1 */

    /* Compute all T(i,k) for i = 1..r, k = 1..c */
    for (idx_t i = 1; i <= r; ++i) {
        dp[i][0] = 0;              /* T(i,0)=0 for i>0 */
        for (idx_t k = 1; k <= c; ++k) {
            seq_t val = dp[i][k-1];
            if (i >= k) val += dp[i-k][k];
            dp[i][k] = val;
        }
    }

    return dp[r][c];
}
