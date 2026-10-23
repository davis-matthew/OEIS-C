/*
 * A144406  Rectangular array A read by upward antidiagonals: entry A(n,k) in
 *          row n and column k gives the number of compositions of k in which
 *          no part exceeds n, n>=1, k>=0.
 * https://oeis.org/A144406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 88, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: reads array A(n,k) of compositions of k with parts ≤ n
 * by upward antidiagonals and returns the n‑th term.  For each n ≥ 1:
 *  • Find diagonal s such that T_{s‑1} < n ≤ T_s (T_m = m(m+1)/2).
 *  • Determine position pos = n – T_{s‑1} (1‑based in the diagonal).
 *  • row = s – pos + 1  (maximum part size), col = pos – 1 (sum k).
 *  • Compute the number of compositions of col with parts ≤ row by dynamic
 *    programming: dp[0] = 1 and for sum = 1..col, dp[sum] = ∑_{p=1}^{min(row,sum)} dp[sum‑p].
 *  • Return dp[col].  The algorithm runs in O(row·col) time; for n ≤ 10 000
 *    row and col are ≤ 141, so the routine is fast. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Locate the diagonal that contains the n-th term. */
    idx_t s   = 1;          /* diagonal index (1‑based) */
    idx_t tri = 1;          /* T_s = s*(s+1)/2 */
    while (tri < n) {
        ++s;
        tri += s;
    }
    /* Now T_s = tri ≥ n.  Compute T_{s‑1} = tri – s. */
    idx_t prev_tri = tri - s;
    idx_t pos      = n - prev_tri;          /* 1‑based position in the diagonal */
    idx_t row      = s - pos + 1;           /* maximum part size */
    idx_t col      = pos - 1;               /* sum to compose */

    /* If col is zero, the empty composition is the only possibility. */
    if (col == 0)
        return (seq_t)1;

    /* Dynamic programming for compositions of 'col' with parts ≤ 'row'. */
    seq_t *dp = (seq_t*)calloc((size_t)(col + 1), sizeof(seq_t));
    if (!dp) /* defensive, though unlikely */
        return (seq_t)0;
    dp[0] = (seq_t)1;

    for (idx_t sum = 1; sum <= col; ++sum) {
        seq_t total = (seq_t)0;
        for (idx_t part = 1; part <= row && part <= sum; ++part)
            total += dp[sum - part];
        dp[sum] = total;
    }

    seq_t result = dp[col];
    free(dp);
    return result;
}
