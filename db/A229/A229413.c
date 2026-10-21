/*
 * A229413  Number of set partitions of {1,...,3n} into sets of size at most
 *          n.
 * https://oeis.org/A229413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of partitions of {1,…,3n} by sets of size ≤ n. The
 *            function uses the classical recurrence dp[m] =
 *            sum_{j=1}^{min(n,m)} C(m-1, j-1) * dp[m-j], with dp[0] = 1,
 *            where dp[m] gives the number of restricted partitions of a set
 *            of size m. The required value is a(n) = dp[3n]
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Index m = 3n : number of elements in the set */
    idx_t m = 3 * n;
    size_t size = (size_t)m + 1;

    /* Allocate DP array */
    seq_t *dp = (seq_t*)malloc(size * sizeof(seq_t));
    if (!dp) return 0;  /* fallback – the caller should never hit OOM */

    dp[0] = 1;  /* empty set has exactly one partition – the empty family */

    /* Build dp[1] … dp[m] */
    for (idx_t k = 1; k <= m; ++k) {
        idx_t maxj = (k < n) ? k : n;   /* block size cannot exceed n or the set size */
        seq_t sum   = 0;                 /* accumulator for dp[k]    */
        seq_t binom = 1;                 /* C(k-1,0) initially 1    */

        for (idx_t j = 1; j <= maxj; ++j) {
            /* term = binom(k-1, j-1) * dp[k-j] */
            seq_t term = dp[k - j];
            term = term * binom;
            sum += term;

            /* Update binom to C(k-1, j) for next iteration:
               binom = binom * (k - j) / j  */
            binom = binom * (k - j);
            binom = binom / j;
        }

        dp[k] = sum;
    }

    seq_t result = dp[m];
    free(dp);
    return result;
}
