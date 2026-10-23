/*
 * A011563  Stirling numbers of second kind S2(14,n).
 * https://oeis.org/A011563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling number of the second kind S(14,n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* offset is 1; values for n<1 are undefined */
    if (n > 14) return 0;          /* S(14,n) = 0 for n>14 */

    static seq_t dp[15][15];
    static bool init = false;
    if (!init) {
        /* dp[i][j] holds S(i,j) for 0<=i<=14, 0<=j<=i */
        dp[0][0] = 1;
        for (idx_t i = 1; i <= 14; ++i) {
            dp[i][0] = 0;
            for (idx_t j = 1; j <= i; ++j) {
                /* recurrence: S(i,j) = S(i-1,j-1) + j * S(i-1,j) */
                dp[i][j] = dp[i-1][j-1] + (seq_t)j * dp[i-1][j];
            }
        }
        init = true;
    }
    return dp[14][n];
}
