/*
 * A158169  Number of nondecreasing integer sequences of length 35 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \sum_{1<=r,s<=35,\;r+s<=35} p_r(n) * p_s(n) where p_k(n)
 *            is the number of partitions of n into exactly k parts. The DP
 *            below computes all p_k(n) in O(n·35)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 142, u128 n <= 222,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#define K 35          /* maximum number of parts in a nondecreasing sequence */

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n>=1; offset 1 */
        return 0;

    /* dp[r][k] = number of partitions of r into parts <= k (unsigned) */
    size_t rows = (size_t)n + 1;
    size_t cols = K + 1;
    size_t total = rows * cols;
    seq_t *dp = (seq_t *)calloc(total, sizeof(seq_t));
    if (!dp)
        return 0;   /* allocation failure – unlikely */

    /* Base case: dp[0][k] = 1 for all k (empty partition) */
    for (size_t k = 0; k < cols; ++k)
        dp[k] = 1;

    /* Fill table using the recurrence:
         dp[r][k] = dp[r][k-1] + (r>=k ? dp[r-k][k] : 0)   */
    for (size_t r = 1; r < rows; ++r) {
        seq_t *row = dp + r * cols;
        for (size_t k = 1; k <= K; ++k) {
            seq_t val = row[k - 1];                     /* dp[r][k-1] */
            if (r >= k)
                val += dp[(r - k) * cols + k];          /* dp[r-k][k] */
            row[k] = val;
        }
    }

    /* pk[k] = number of partitions of n into exactly k parts */
    seq_t pk[K + 1];
    pk[0] = 0;
    for (size_t k = 1; k <= K; ++k) {
        seq_t withk   = dp[(size_t)n * cols + k];
        seq_t withoutk = dp[(size_t)n * cols + k - 1];
        pk[k] = withk - withoutk;
    }

    /* Sum over admissible pairs (r,s) with r+s <= 35 */
    seq_t result = 0;
    for (size_t r = 1; r <= K; ++r) {
        seq_t val_r = pk[r];
        if (!val_r) continue;
        size_t max_s = K - r;
        for (size_t s = 1; s <= max_s; ++s) {
            seq_t val_s = pk[s];
            if (!val_s) continue;
            result += val_r * val_s;
        }
    }

    free(dp);
    return result;
}
