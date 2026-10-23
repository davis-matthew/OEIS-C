/*
 * A153362  Number of zig-zag paths from top to bottom of a rectangle of width
 *          9 with n rows.
 * https://oeis.org/A153362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: Let dp[i] be the number of length‑k words ending with symbol i
   (0 ≤ i ≤ 8).  Start with dp[i] = 1 for k = 1.
   For each subsequent length k + 1,
       dp_next[i] = (i > 0 ? dp[i - 1] : 0) + (i < 8 ? dp[i + 1] : 0).
   The count for length n is sum of dp[i] after n steps. *
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)              /* no defined value, but safe to return 0    */
        return 0;
    /* current counts for each symbol 0..8 */
    seq_t cur[9];
    seq_t nxt[9];
    for (idx_t i = 0; i < 9; ++i)
        cur[i] = 1;          /* words of length 1           */

    if (n == 1)
        return 9;            /* 9 single‑letter words       */

    for (idx_t step = 2; step <= n; ++step)
    {
        for (idx_t i = 0; i < 9; ++i)
        {
            seq_t left  = (i > 0)   ? cur[i - 1] : 0;
            seq_t right = (i < 8)   ? cur[i + 1] : 0;
            nxt[i] = left + right;
        }
        /* copy nxt into cur for next iteration */
        for (idx_t i = 0; i < 9; ++i)
            cur[i] = nxt[i];
    }

    seq_t result = 0;
    for (idx_t i = 0; i < 9; ++i)
        result += cur[i];
    return result;
}
