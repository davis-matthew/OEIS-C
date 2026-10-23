/*
 * A107480  a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5) + a(n-7).
 * https://oeis.org/A107480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = a(n-1) + a(n-3) + a(n-4) + a(n-5) + a(n-7), with initial
 *            values a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=8
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 43, u64 n <= 85, u128 n <= 168,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoisation array */
    static seq_t *dp = NULL;           /* dynamic array of computed terms   */
    static idx_t  dp_size = 0;         /* current allocated capacity       */
    static int    init   = 0;          /* whether the first seven values are set */

    /* Ensure resizable array holds indices 0..n */
    if (dp == NULL || dp_size <= n) {
        /* Grow capacity: double or at least 64 elements */
        idx_t new_size = dp_size ? (dp_size << 1) : 64;
        if (new_size <= n) new_size = n + 1;
        /* realloc must be cast to seq_t* when seq_t is a class (C++). */
        dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        dp_size = new_size;
    }

    /* Initialise first seven terms once */
    if (!init) {
        dp[0] = 0;
        dp[1] = 1;
        dp[2] = 1;
        dp[3] = 2;
        dp[4] = 3;
        dp[5] = 5;
        dp[6] = 8;
        init = 1;
    }

    /* Compute up to n, if needed */
    for (idx_t i = 7; i <= n; ++i) {
        /* dp[i] = dp[i-1] + dp[i-3] + dp[i-4] + dp[i-5] + dp[i-7] */
        seq_t sum = dp[i - 1];
        sum += dp[i - 3];
        sum += dp[i - 4];
        sum += dp[i - 5];
        sum += dp[i - 7];
        dp[i] = sum;
    }

    return dp[n];
}
