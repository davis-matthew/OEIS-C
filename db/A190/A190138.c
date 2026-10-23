/*
 * A190138  Final number of terms obtained with Euler's recurrence formula
 *          when computing the sum of divisors of n.
 * https://oeis.org/A190138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n using generalized pentagonal
 *            numbers (k(3k±1)/2 with k ≥ 1)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* DP table: dp[i] = a(i) */
    static idx_t dp_max = 0;      /* largest n already computed */

    if (n == 0) return 0;           /* the sequence is defined for n ≥ 1 */

    /* Allocate or grow the DP table as needed */
    if (dp == NULL) {
        dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (dp == NULL) return 0;  /* allocation failure – unlikely in tests */
        dp[0] = 1;                 /* base case: empty composition */
        dp_max = 0;
    } else if (dp_max < n) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (tmp == NULL) return 0;/* allocation failure */
        dp = tmp;
        /* existing entries up to dp_max remain untouched */
    }

    /* Compute values up to n */
    for (idx_t i = dp_max + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t p1 = k * (3 * k - 1) / 2;   /* k(3k-1)/2 */
            if (p1 > i) break;               /* all further p1 are larger */
            sum += dp[i - p1];

            idx_t p2 = k * (3 * k + 1) / 2;   /* k(3k+1)/2 */
            if (p2 <= i) sum += dp[i - p2];
        }
        dp[i] = sum;
    }

    dp_max = n;
    return dp[n];
}
