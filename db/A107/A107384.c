/*
 * A107384  a(n)= a(n-1) +3*a(n-2) -3*a(n-4).
 * https://oeis.org/A107384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - 3*a(n-4); a(0)=0, a(1)=1, a(2)=1,
 *            a(3)=2
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;
    static idx_t size = 0;

    /* Initialize base values */
    if (size == 0) {
        size = 4;                /* we store indices 0..3 */
        dp = (seq_t *)malloc(sizeof(seq_t) * size);
        if (!dp) return 0;       /* allocation failure (unlikely) */
        dp[0] = 0;
        dp[1] = 1;
        dp[2] = 1;
        dp[3] = 2;
    }

    /* Extend the table if the requested index is beyond what we have */
    if (n >= size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(dp, sizeof(seq_t) * new_size);
        if (!tmp) return 0;     /* allocation failure (unlikely) */
        dp = tmp;
        for (idx_t i = size; i <= n; ++i) {
            dp[i] = dp[i-1] + (seq_t)3 * dp[i-2] - (seq_t)3 * dp[i-4];
        }
        size = new_size;
    }

    return dp[n];
}
