/*
 * A164137  Number of binary strings of length n with equal numbers of 000 and
 *          001 substrings.
 * https://oeis.org/A164137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of binary strings of length n whose counts of
 *            substrings 000 and 001 are equal. This implementation uses
 *            dynamic programming on the last two symbols and the difference
 *            (#000 − #001). No negative numbers appear – the difference is
 *            represented as a non‑negative index in a fixed window. The...
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>

/* Maximum difference between the numbers of 000 and 001 substrings
   cannot exceed n‑2, so 20000 is sufficient for n up to 10000. */
#define MAX_DIFF 20000
#define OFF      MAX_DIFF                      /* index for difference 0 */

static seq_t dp[2][4][2 * MAX_DIFF + 1];
static int   stamp[4][2 * MAX_DIFF + 1];

seq_t a(idx_t n)
{
    /* clear the buffers for each call */
    memset(dp, 0, sizeof(dp));
    memset(stamp, 0, sizeof(stamp));

    /* handle the trivial cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* ----------------------------------------------------------------
       Initialise the DP table for length 2.  Every 2‑bit suffix occurs
       exactly once and the difference is 0 (index OFF).
       ---------------------------------------------------------------- */
    int cur = 0, next = 1;
    for (int s = 0; s < 4; ++s)
        dp[cur][s][OFF] = (seq_t)1;

    /* ranges of active indices for each suffix, at the current length */
    int minD[4], maxD[4];
    for (int s = 0; s < 4; ++s) {
        minD[s] = OFF;
        maxD[s] = OFF;
    }

    /* step counter for the stamping mechanism in this call */
    idx_t step_id = 1;

    /* --------------------------------------------------------------
       Extend the strings from length 2 up to length n.
       -------------------------------------------------------------- */
    for (idx_t len = 2; len < n; ++len) {
        ++step_id;
        int newMin[4], newMax[4];
        for (int s = 0; s < 4; ++s) {
            newMin[s] = 2 * MAX_DIFF + 1;   /* one past the valid range */
            newMax[s] = -1;
        }

        for (int s = 0; s < 4; ++s) {
            for (int d = minD[s]; d <= maxD[s]; ++d) {
                seq_t val = dp[cur][s][d];
                if (val == 0) continue;

                switch (s) {
                    case 0: /* suffix 00 */
                        /* append 0 -> new suffix 00, diff +1 */
                        {
                            int nd = d + 1;
                            if (stamp[0][nd] != step_id) {
                                stamp[0][nd] = (int)step_id;
                                dp[next][0][nd] = val;
                            } else
                                dp[next][0][nd] += val;
                            if (nd < newMin[0]) newMin[0] = nd;
                            if (nd > newMax[0]) newMax[0] = nd;
                        }
                        /* append 1 -> new suffix 01, diff -1 */
                        {
                            int nd = d - 1;
                            if (stamp[1][nd] != step_id) {
                                stamp[1][nd] = (int)step_id;
                                dp[next][1][nd] = val;
                            } else
                                dp[next][1][nd] += val;
                            if (nd < newMin[1]) newMin[1] = nd;
                            if (nd > newMax[1]) newMax[1] = nd;
                        }
                        break;
                    case 1: /* suffix 01 */
                        /* append 0 -> suffix 10, diff unchanged */
                        {
                            int nd = d;
                            if (stamp[2][nd] != step_id) {
                                stamp[2][nd] = (int)step_id;
                                dp[next][2][nd] = val;
                            } else
                                dp[next][2][nd] += val;
                            if (nd < newMin[2]) newMin[2] = nd;
                            if (nd > newMax[2]) newMax[2] = nd;
                        }
                        /* append 1 -> suffix 11, diff unchanged */
                        {
                            int nd = d;
                            if (stamp[3][nd] != step_id) {
                                stamp[3][nd] = (int)step_id;
                                dp[next][3][nd] = val;
                            } else
                                dp[next][3][nd] += val;
                            if (nd < newMin[3]) newMin[3] = nd;
                            if (nd > newMax[3]) newMax[3] = nd;
                        }
                        break;
                    case 2: /* suffix 10 */
                        /* append 0 -> suffix 00, diff unchanged */
                        {
                            int nd = d;
                            if (stamp[0][nd] != step_id) {
                                stamp[0][nd] = (int)step_id;
                                dp[next][0][nd] = val;
                            } else
                                dp[next][0][nd] += val;
                            if (nd < newMin[0]) newMin[0] = nd;
                            if (nd > newMax[0]) newMax[0] = nd;
                        }
                        /* append 1 -> suffix 01, diff unchanged */
                        {
                            int nd = d;
                            if (stamp[1][nd] != step_id) {
                                stamp[1][nd] = (int)step_id;
                                dp[next][1][nd] = val;
                            } else
                                dp[next][1][nd] += val;
                            if (nd < newMin[1]) newMin[1] = nd;
                            if (nd > newMax[1]) newMax[1] = nd;
                        }
                        break;
                    case 3: /* suffix 11 */
                        /* append 0 -> suffix 10, diff unchanged */
                        {
                            int nd = d;
                            if (stamp[2][nd] != step_id) {
                                stamp[2][nd] = (int)step_id;
                                dp[next][2][nd] = val;
                            } else
                                dp[next][2][nd] += val;
                            if (nd < newMin[2]) newMin[2] = nd;
                            if (nd > newMax[2]) newMax[2] = nd;
                        }
                        /* append 1 -> suffix 11, diff unchanged */
                        {
                            int nd = d;
                            if (stamp[3][nd] != step_id) {
                                stamp[3][nd] = (int)step_id;
                                dp[next][3][nd] = val;
                            } else
                                dp[next][3][nd] += val;
                            if (nd < newMin[3]) newMin[3] = nd;
                            if (nd > newMax[3]) newMax[3] = nd;
                        }
                        break;
                }
            }
        }

        /* prepare for the next step */
        for (int s = 0; s < 4; ++s) {
            minD[s] = newMin[s];
            maxD[s] = newMax[s];
        }
        int tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum the counts of strings with difference 0 (index OFF) */
    seq_t result = 0;
    for (int s = 0; s < 4; ++s)
        result += dp[cur][s][OFF];
    return result;
}
