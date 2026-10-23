/*
 * A026825  Number of partitions of n into distinct parts, the least being 4.
 * https://oeis.org/A026825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts, the least
 *            part being 4. For n<4 the value is 0. The implementation uses a
 *            straightforward dynamic‑programming routine that builds, for
 *            each required m = n-4, the number of ways to represent m as a
 *            sum of distinct integers ≥5. It stores a table...
 * Range:     u8 n <= 50, u16 n <= 109, u32 n <= 288, u64 n <= 881,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;     /* dp[s] = # of subsets of {5,6,…} summing to s */
static idx_t max_m = 0;      /* largest m = n-4 that has been precomputed */

seq_t a(idx_t n)
{
    if (n < 4)      /* a(0)=a(1)=a(2)=a(3)=0                             */
        return 0;

    idx_t m = n - 4;          /* target sum using parts ≥5                    */

    if (dp == NULL || m > max_m) {
        /* Allocate / expand the DP array to size m+1 (indices 0 … m).   */
        seq_t *new_dp = (seq_t *)realloc(dp,
                                          (size_t)(m + 1) * sizeof(seq_t));
        if (!new_dp) {
            /* Allocation failed – terminate the program.                */
            exit(EXIT_FAILURE);
        }
        dp = new_dp;

        /* Initialise all entries and set the base case.                 */
        for (idx_t i = 0; i <= m; ++i)
            dp[i] = 0;
        dp[0] = 1;   /* empty set sums to 0                               */

        /* Dynamic programming: iterate over each allowed part.         */
        for (idx_t part = 5; part <= m; ++part) {
            for (idx_t sum = m; sum >= part; --sum) {
                dp[sum] += dp[sum - part];
            }
        }
        max_m = m;
    }

    return dp[m];
}
