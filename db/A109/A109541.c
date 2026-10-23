/*
 * A109541  a(n) = a(n-2)+a(n-3)+a(n-4)+a(n-5)+2*a(n-6)+a(n-7).
 * https://oeis.org/A109541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)+a(n-5)+2*a(n-6)+a(n-7) Initial
 *            values: a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=1, a(6)=1,
 *            a(7)=7
 * Range:     u8 n <= 15, u16 n <= 26, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state to memoize the sequence up to the largest requested n */
    static seq_t *dp = NULL;   /* array holding computed values */
    static idx_t  dp_size = 0; /* current length of the array */

    /* if needed, grow the array to hold values up to n */
    if (dp_size <= n)
    {
        idx_t new_size = n + 1;
        dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        /* compute values from the previously computed size up to n */
        for (idx_t i = dp_size; i <= n; ++i)
        {
            if (i <= 6)          /* base values a(0)..a(6) = 1 */
                dp[i] = 1;
            else if (i == 7)      /* a(7) = 7 */
                dp[i] = 7;
            else /* i >= 8 : recurrence */
                dp[i] = dp[i-2] + dp[i-3] + dp[i-4] + dp[i-5]
                      + 2 * dp[i-6] + dp[i-7];
        }
        dp_size = new_size;
    }

    return dp[n];
}
