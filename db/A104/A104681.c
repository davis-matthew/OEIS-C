/*
 * A104681  Expansion of
 *          (1-x-2*x^2-2*x^3-9*x^4-9*x^5-6*x^6+6*x^7-x^8-x^9-2*x^13+2*x^12) /
 *          (-x^12-1+2*x^6).
 * https://oeis.org/A104681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*a(n-6) - a(n-12) for n>13 with initial values a(0) =
 *            -1, a(1) = 1, a(2) = 2, a(3) = 2, a(4) = 9, a(5) = 9, a(6) = 4,
 *            a(7) = -4, a(8) = 5, a(9) = 5, a(10) = 18, a(11) = 18, a(12) =
 *            7, a(13) = -7
 * Range:     i8 n <= 87, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;
static idx_t dp_size = 0;

seq_t a(idx_t n)
{
    /* initialise base cases once */
    if (dp == NULL) {
        dp_size = 14;                            /* indices 0..13 */
        dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
        if (!dp) return 0;                       /* allocation failed */
        dp[0]  = (seq_t)(-1);
        dp[1]  = (seq_t)1;
        dp[2]  = (seq_t)2;
        dp[3]  = (seq_t)2;
        dp[4]  = (seq_t)9;
        dp[5]  = (seq_t)9;
        dp[6]  = (seq_t)4;
        dp[7]  = (seq_t)(-4);
        dp[8]  = (seq_t)5;
        dp[9]  = (seq_t)5;
        dp[10] = (seq_t)18;
        dp[11] = (seq_t)18;
        dp[12] = (seq_t)7;
        dp[13] = (seq_t)(-7);
    }

    /* extend table if needed */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp) return 0;                    /* allocation failed */
        dp = new_dp;
        /* compute new entries using the recurrence */
        for (idx_t i = dp_size; i < new_size; ++i) {
            if (i >= 14) {
                dp[i] = 2 * dp[i-6] - dp[i-12];
            }
        }
        dp_size = new_size;
    }

    return dp[n];
}
