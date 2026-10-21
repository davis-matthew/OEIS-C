/*
 * A233412  c-analog of Euler phi-function: a(n) is number of nonnegative
 *          integers not exceeding n and c-prime to n.
 * https://oeis.org/A233412
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of integers k in [0,n] with c‑GCD(k,n)=0 The
 *            implementation counts numbers k whose binary “parts” (1 followed
 *            by a run of zeros) have no run length equal to any run length
 *            that occurs in the binary parts of n itself. This is done by a
 *            digit DP over the bits of n
 * Range:     u8 n <= 255, u16 n <= 4999, u32 n <= 4999, u64 n <= 4999,
 *            u128 n <= 4999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static idx_t N_global;
static int BITLEN;
static bool forb[65];
static seq_t memo[65][65][2][2];
static bool vis[65][65][2][2];

/* Recursive DP: position pos (0 = LSB), zeroCount = number of zeros
   since the last 1, lastOne = 1 if a 1 has appeared before, tight = 1
   if bits so far equal the prefix of N_global. */
static seq_t dfs(int pos, int zeroCount, int lastOne, int tight) {
    if (pos < 0) {
        if (lastOne == 0) return (seq_t)1;            /* number 0 */
        return forb[zeroCount] ? (seq_t)0 : (seq_t)1; /* trailing zeros check */
    }
    if (vis[pos][zeroCount][lastOne][tight]) return memo[pos][zeroCount][lastOne][tight];
    vis[pos][zeroCount][lastOne][tight] = true;

    seq_t ans = 0;
    int limit = tight ? ((N_global >> pos) & 1) : 1;
    for (int b = 0; b <= limit; ++b) {
        int newTight = (tight && (b == limit));
        if (lastOne == 0) {
            if (b == 0)
                ans += dfs(pos - 1, zeroCount, 0, newTight);
            else
                ans += dfs(pos - 1, 0, 1, newTight);
        } else {
            if (b == 0)
                ans += dfs(pos - 1, zeroCount + 1, 1, newTight);
            else {
                if (!forb[zeroCount])          /* zeroCount is the run after previous 1 */
                    ans += dfs(pos - 1, 0, 1, newTight);
            }
        }
    }
    memo[pos][zeroCount][lastOne][tight] = ans;
    return ans;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;   /* a(0)=1 */

    /* find highest set bit */
    int high = 63;
    while (high >= 0 && ((n >> high) & 1) == 0) high--;
    BITLEN = high + 1;

    /* clear forbidden table */
    memset(forb, 0, sizeof(forb));

    /* collect positions of 1s from high to low */
    int posList[64];
    int cnt = 0;
    for (int i = high; i >= 0; --i)
        if ((n >> i) & 1) posList[cnt++] = i;

    /* compute zero run lengths for parts of n */
    for (int i = 0; i < cnt; ++i) {
        int cur = posList[i];
        int next = (i + 1 < cnt) ? posList[i + 1] : -1;
        int zeroRun = (next == -1) ? cur : cur - next - 1;
        forb[zeroRun] = true;
    }

    /* initialize DP memoisation table */
    memset(vis, 0, sizeof(vis));

    N_global = n;
    return dfs(high, 0, 0, 1);
}
