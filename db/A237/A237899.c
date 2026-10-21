/*
 * A237899  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the upper
 *          median minus the minimum of every 2X2 subblock differing from its
 *          horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)x2 arrays over {0,1,2,3} whose 2x2
 *            subblock value max+upperMedian-min differs by exactly one from
 *            the one immediately above it. Computed by a dynamic‑programming
 *            over row pairs
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset 1: a(1) is first term */

    /* pre‑compute the block value for every pair of rows */
    static unsigned char vmat[16][16];
    static bool init = false;
    if (!init) {
        for (int s = 0; s < 16; ++s) {
            int a =  s & 3;
            int b =  s >> 2;
            for (int t = 0; t < 16; ++t) {
                int c =  t & 3;
                int d =  t >> 2;
                int arr[4] = {a,b,c,d};

                int mn = arr[0], mx1 = arr[0], mx2 = -1;
                for (int k = 1; k < 4; ++k) {
                    int x = arr[k];
                    if (x < mn) mn = x;
                    if (x > mx1) { mx2 = mx1; mx1 = x; }
                    else if (x > mx2) mx2 = x;
                }
                vmat[s][t] = (unsigned char)(mx1 + mx2 - mn);
            }
        }
        init = true;
    }

    /* DP arrays: dp[row][prev_value] */
    seq_t dp[16][7] = {0};
    seq_t newdp[16][7];

    /* first block */
    for (int r0 = 0; r0 < 16; ++r0)
        for (int r1 = 0; r1 < 16; ++r1)
            dp[r1][vmat[r0][r1]] += (seq_t)1;

    /* subsequent blocks */
    if (n > 1) {
        for (idx_t step = 1; step < n; ++step) {
            /* zero newdp */
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 7; ++j)
                    newdp[i][j] = (seq_t)0;

            for (int cur = 0; cur < 16; ++cur)
                for (int pv = 0; pv < 7; ++pv) {
                    seq_t cnt = dp[cur][pv];
                    if (cnt == 0) continue;
                    for (int nxt = 0; nxt < 16; ++nxt) {
                        unsigned char nv = vmat[cur][nxt];
                        int diff = (pv > nv) ? pv - nv : nv - pv;
                        if (diff == 1)
                            newdp[nxt][nv] += cnt;
                    }
                }

            /* move newdp to dp */
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 7; ++j)
                    dp[i][j] = newdp[i][j];
        }
    }

    /* sum all counts */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 7; ++j)
            result += dp[i][j];

    return result;
}
