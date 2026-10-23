/*
 * A107383  a(n) = 2*a(n-2) + 2*a(n-3).
 * https://oeis.org/A107383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 2*a(n-3), a(0)=0, a(1)=1, a(2)=1
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 40, u64 n <= 79, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp   = NULL; /* dynamic array of computed terms */
    static idx_t dp_size = 0;  /* number of terms stored */
    static idx_t dp_cap   = 0; /* current capacity of dp */

    /* Initialize on first call */
    if (dp == NULL)
    {
        dp_cap = 8;                 /* initial allocation */
        dp = (seq_t *)malloc(dp_cap * sizeof(seq_t));
        if (!dp) return (seq_t)0;   /* allocation failure */
        dp[0] = (seq_t)0;
        dp[1] = (seq_t)1;
        dp[2] = (seq_t)1;
        dp_size = 3;
    }

    /* Grow the array if needed */
    if ((idx_t)n >= dp_cap)
    {
        idx_t new_cap = dp_cap;
        while ((idx_t)n >= new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        dp = tmp;
        dp_cap = new_cap;
    }

    /* Compute missing terms up to n */
    if ((idx_t)n >= dp_size)
    {
        for (idx_t i = dp_size; i <= n; ++i)
        {
            if (i < 3)            /* safety for small indices */
            {
                if (i == 0) dp[i] = (seq_t)0;
                else if (i == 1) dp[i] = (seq_t)1;
                else             dp[i] = (seq_t)1;
            }
            else
            {
                dp[i] = (seq_t)2 * dp[i-2] + (seq_t)2 * dp[i-3];
            }
        }
        dp_size = n + 1;
    }

    return dp[n];
}
