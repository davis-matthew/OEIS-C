/*
 * A221540  Number of 0..6 arrays of length n with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Sequence A221540
 *   Count of length-n arrays over alphabet {0..6} starting with 0
 *   such that every element has at least one neighbor whose
 *   difference to it is not 1 (for interior elements) or, for
 *   the ends, the single neighbor has a difference not 1.
 *
 *   Implementation: dynamic programming over the last element value
 *   and a flag indicating whether that element already satisfied its
 *   condition with the left neighbor.  Complexity: O(n) time, O(1) memory.
 */
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 1) return 0;
    if (n == 2) return 6;

    /* dp[val][flag] – number of ways for the current prefix
     * (ending with value val) where flag==1 means the
     * last element already satisfies the condition via its
     * left neighbor, flag==0 means not yet. */
    seq_t dp[7][2] = {{0}};
    seq_t dp_next[7][2] = {{0}};

    /* initialise for the second element (position 2) */
    for (idx_t v = 0; v < 7; ++v) {
        if (v == 1) continue;          /* a[2]==1 would force a[1] to differ by 1 */
        /* left neighbor is 0; diff = v.  Since v != 1, the flag is set. */
        dp[v][1] = 1;
    }

    /* iterate positions 3 .. n */
    for (idx_t pos = 3; pos <= n; ++pos) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int prev = 0; prev < 7; ++prev) {
            for (int f = 0; f < 2; ++f) {
                seq_t cnt = dp[prev][f];
                if (cnt == 0) continue;

                for (int curr = 0; curr < 7; ++curr) {
                    /* check that the previous element satisfies its condition */
                    if (f == 0) {
                        /* must not have |curr - prev| == 1 */
                        int d = curr - prev;
                        if (d < 0) d = -d;
                        if (d == 1) continue;
                    }

                    /* set flag for the current element */
                    int f_next = 0;
                    {
                        int d = curr - prev;
                        if (d < 0) d = -d;
                        if (d != 1) f_next = 1;
                    }
                    dp_next[curr][f_next] += cnt;
                }
            }
        }
        /* copy dp_next back to dp for the next iteration */
        for (int i = 0; i < 7; ++i) {
            dp[i][0] = dp_next[i][0];
            dp[i][1] = dp_next[i][1];
        }
    }

    /* final element must be satisfied via its left neighbor */
    seq_t result = 0;
    for (int v = 0; v < 7; ++v) {
        result += dp[v][1];
    }
    return result;
}
