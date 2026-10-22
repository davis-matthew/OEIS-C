/*
 * A007785  Number of sets of positive integers <= n^2 whose sum is (n^3 +
 *          n)/2.
 * https://oeis.org/A007785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1..n^2} with sum n*(n^2+1)/2,
 *            computed by 0/1 knapsack subset-sum counting DP; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;

    size_t nn = (size_t)n;
    size_t target = (nn * nn * nn + nn) / 2;
    size_t max_elem = nn * nn;
    if (max_elem > target) max_elem = target;

    seq_t *dp = (seq_t *)malloc((target + 1) * sizeof(seq_t));
    if (!dp) return 0;
    for (size_t s = 0; s <= target; s++) dp[s] = 0;
    dp[0] = 1;

    for (size_t e = 1; e <= max_elem; e++) {
        for (size_t s = target; s >= e; s--) {
            dp[s] += dp[s - e];
        }
    }

    seq_t result = dp[target];
    free(dp);
    return result;
}
