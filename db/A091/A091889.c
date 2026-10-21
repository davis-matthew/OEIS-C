/*
 * A091889  Number of partitions of n into sums of exactly two distinct powers
 *          of 2.
 * https://oeis.org/A091889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into sums of exactly two
 *            distinct powers of 2. The algorithm enumerates all part weights
 *            2^i + 2^j (i < j, 2^i + 2^j ≤ n) and uses an unrestricted
 *            knapsack DP to count unordered partitions
 * Range:     u8 n <= 47, u16 n <= 121, u32 n <= 394, u64 n <= 1655,
 *            u128 n <= 9344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The usual OEIS helper headers are not needed for this simple routine. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0. */
    if (n == 0) return 0;

    /* Compute the largest exponent needed: floor(log2(n)).  We
       increment max_exp until 2^(max_exp+1) > n. */
    idx_t max_exp = 0;
    while (((idx_t)1ULL << (max_exp + 1)) <= n) max_exp++;

    /* Generate all eligible part weights w = 2^i + 2^j. */
    static seq_t weights[256];        /* ample size for n up to 2^32 */
    size_t wcnt = 0;

    for (idx_t i = 0; i <= max_exp; ++i) {
        seq_t vi = (seq_t)1 << i;
        for (idx_t j = i + 1; j <= max_exp; ++j) {
            seq_t vj = (seq_t)1 << j;
            seq_t w  = vi + vj;
            if (w > (seq_t)n) break;   /* larger j only increases w */
            weights[wcnt++] = w;
        }
    }

    /* Unbounded knapsack DP: dp[s] = number of partitions of s. */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!dp) return 0;            /* allocation failure → 0 as safe */
    dp[0] = 1;

    for (size_t wi = 0; wi < wcnt; ++wi) {
        idx_t w_idx = (idx_t)weights[wi]; /* w ≤ n, so fits in idx_t */
        for (idx_t s = w_idx; s <= n; ++s) {
            dp[s] += dp[s - w_idx];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
