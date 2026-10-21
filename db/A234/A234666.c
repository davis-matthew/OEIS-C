/*
 * A234666  Number of combinations for the sum of 6 different numbers from 1
 *          to 49.
 * https://oeis.org/A234666
 *
 * Offset:    21    (a(21) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 103, u32 n <= 279, u64 n <= 279,
 *            u128 n <= 279, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of ways to choose 6 distinct numbers from 1..49 whose sum equals n
   (OEIS A234666, offset 21).  For n < 21 or n > 279 the value is 0. */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* table[s] will hold the result for sum s (0 ≤ s ≤ 279) */
    static seq_t table[280];
    static bool built = false;

    if (!built) {
        /* dynamic programming table:
           dp[k][s] = number of ways to choose k distinct numbers ≤ current
           value whose sum is s.  We need k = 0..6 and s = 0..279. */
        seq_t dp[7][280];

        /* initialise to zero */
        for (idx_t i = 0; i < 7; ++i)
            for (idx_t j = 0; j < 280; ++j)
                dp[i][j] = 0;

        dp[0][0] = 1;  /* one way to choose nothing with sum 0 */

        /* iterate over the numbers 1..49, adding them to the DP table */
        for (idx_t num = 1; num <= 49; ++num) {
            /* decrement k to avoid re‑using the same number in a single
               combination.  Unsigned loop is safe because the termination
               condition is k >= 1. */
            for (idx_t k = 6; k >= 1; --k) {
                for (idx_t s = 279; s >= num; --s) {
                    dp[k][s] += dp[k - 1][s - num];
                }
            }
        }

        /* copy the final counts for k = 6 into the static table.  A plain
           assignment loop is used instead of memcpy because seq_t may be a
           non‑POD arbitrary‑precision type. */
        for (idx_t i = 0; i < 280; ++i)
            table[i] = dp[6][i];

        built = true;
    }

    if (n < 21 || n > 279)
        return 0;
    return table[n];
}
