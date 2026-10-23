/*
 * A109021  a(n) = (2*7^n - 6*3^n + 4)/6.
 * https://oeis.org/A109021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*7^n - 6*3^n + 4)/6 Recurrence: a(0)=0, a(1)=0, a(2)=8,
 *            a(n)=11*a(n-1)-31*a(n-2)+21*a(n-3) for n≥3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;     /* memoization array */
    static idx_t dp_size = 0;    /* number of computed entries */

    /* If we already have the value, return it immediately */
    if (n < dp_size) return dp[n];

    /* Allocate (or enlarge) the array up to n+1 */
    seq_t *new_dp = (seq_t *)realloc(dp, (n + 1) * sizeof(seq_t));
    if (!new_dp) abort();  /* allocation failure */

    /* Initialize the new portion to zero (values will be set below) */
    for (idx_t i = dp_size; i <= n; ++i) new_dp[i] = 0;

    /* Base cases */
    new_dp[0] = 0;
    if (n >= 1) new_dp[1] = 0;
    if (n >= 2) new_dp[2] = 8;

    /* Compute missing terms using the recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = 11 * new_dp[i - 1];
        seq_t t2 = 31 * new_dp[i - 2];
        seq_t t3 = 21 * new_dp[i - 3];
        new_dp[i] = t1 - t2 + t3;
    }

    /* Update static storage */
    dp = new_dp;
    dp_size = n + 1;

    return dp[n];
}
