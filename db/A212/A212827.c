/*
 * A212827  Number of 0..6 arrays of length n with no adjacent pair equal to
 *          its immediately preceding adjacent pair, and new values introduced
 *          in 0..6 order.
 * https://oeis.org/A212827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 16*a(n-1) -79*a(n-2) +70*a(n-3) +361*a(n-4 
 *             -372*a(n-5) -964*a(n-6)+144*a(n-7)+1116*a(n-8)+720*a(n-9)+144*a(n-10) 
 *             for n > 13
 * Initial values: 
 * a(1) = 1, a(2) = 2, a(3) = 5, a(4) = 13, a(5) = 44, a(6) = 168,
 * a(7) = 717, a(8) = 3359, a(9) = 17055, a(10) = 92749, a(11) = 534071,
 * a(12) = 3220152, a(13) = 20122695
 *
 * The sequence counts 0..6 arrays of length n with:
 * 1) no adjacent pair equal to its immediately preceding adjacent pair
 * (i.e. no pattern XYXY in the array),
 * 2) new values introduced in increasing order 0,1,2,3,4,5,6.
 *
 * We're computing it by dynamic programming.
 * State: dp[k][x][y][z] = number of arrays of current length
 *        with last three symbols x (position-2), y (position-1), z (position),
 *        and distinct symbols used = k (k = max symbol + 1).
 *
 * Transitions:
 *   add new symbol w.
 *   If w is existing (0 ≤ w < k):
 *     if (x == z && y == w) discard (would create pattern XYXY).
 *     else dp_next[k][y][z][w] += dp[k][x][y][z].
 *   If w == k (new symbol) and k < 7:
 *     if (x == z && y == k) discard.
 *     else dp_next[k+1][y][z][k] += dp[k][x][y][z].
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* DP arrays: dp[k][x][y][z], indices used only for 1 ≤ k ≤ 7, 0 ≤ x,y,z < k */
    static seq_t dp[8][7][7][7];
    static seq_t dp_next[8][7][7][7];

    /* clear dp and dp_next */
    for (int k = 1; k <= 7; ++k)
        for (int x = 0; x < k; ++x)
            for (int y = 0; y < k; ++y)
                for (int z = 0; z < k; ++z) {
                    dp[k][x][y][z]     = 0;
                    dp_next[k][x][y][z] = 0;
                }

    /* initialize sequences of length 3 (RGFs of length 3) */
    dp[1][0][0][0] = 1;           /* 0,0,0 */
    dp[2][0][0][1] = 1;           /* 0,0,1 */
    dp[2][0][1][0] = 1;           /* 0,1,0 */
    dp[2][0][1][1] = 1;           /* 0,1,1 */
    dp[3][0][1][2] = 1;           /* 0,1,2 */

    /* iterate lengths from 3 up to n-1 to build up to length n */
    for (idx_t len = 3; len < n; ++len) {
        /* zero dp_next */
        for (int k = 1; k <= 7; ++k)
            for (int x = 0; x < k; ++x)
                for (int y = 0; y < k; ++y)
                    for (int z = 0; z < k; ++z)
                        dp_next[k][x][y][z] = 0;

        for (int k = 1; k <= 7; ++k) {
            for (int x = 0; x < k; ++x) {
                for (int y = 0; y < k; ++y) {
                    for (int z = 0; z < k; ++z) {
                        seq_t cnt = dp[k][x][y][z];
                        if (cnt == 0) continue;

                        /* existing symbols */
                        for (int w = 0; w < k; ++w) {
                            if (x == z && y == w) continue;          /* skip pattern XYXY */
                            dp_next[k][y][z][w] += cnt;
                        }

                        /* new symbol */
                        if (k < 7) {
                            int w = k;                                /* new symbol value */
                            if (!(x == z && y == w)) {
                                dp_next[k + 1][y][z][w] += cnt;
                            }
                        }
                    }
                }
            }
        }

        /* copy dp_next to dp for next iteration */
        for (int k = 1; k <= 7; ++k)
            for (int x = 0; x < k; ++x)
                for (int y = 0; y < k; ++y)
                    for (int z = 0; z < k; ++z)
                        dp[k][x][y][z] = dp_next[k][x][y][z];
    }

    /* sum all counts for sequences of length n */
    seq_t result = 0;
    for (int k = 1; k <= 7; ++k)
        for (int x = 0; x < k; ++x)
            for (int y = 0; y < k; ++y)
                for (int z = 0; z < k; ++z)
                    result += dp[k][x][y][z];

    return result;
}
