/*
 * A185553  1/3 the number of nX3 0..2 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   good_row_no_down, good_row_only_down, good_row_only_horiz
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A185553
   a(n) = (number of n×3 arrays with entries 0..2 in which each
          element equals exactly one or two of its horizontal and vertical
          neighbours) / 3.
   The program counts the arrays using DP over the 27 possible
   row‑states and divides the final count by 3. */

#include <stdbool.h>
#include "../utils/good_row_no_down.h"
#include "../utils/good_row_only_down.h"
#include "../utils/good_row_only_horiz.h"

/* ------------------------------------------------------------------- */
/* Row validation helpers                                                */
/* ------------------------------------------------------------------- */

/* test if the middle row (mid) satisfies the condition when surrounded
   by the up and down rows.  All horizontal, up and down neighbours are
   considered. */
static inline bool good_row(int mid, int up, int down)
{
    static int decode[27][3];
    static bool init = false;
    if (!init) {
        for (int code = 0; code < 27; ++code) {
            int v = code;
            decode[code][0] = v % 3;  v /= 3;
            decode[code][1] = v % 3;  v /= 3;
            decode[code][2] = v;
        }
        init = true;
    }
    const int *midv  = decode[mid];
    const int *upv   = decode[up];
    const int *downv = decode[down];
    for (int j = 0; j < 3; ++j) {
        int val   = midv[j];
        int count = 0;
        if (upv[j]      == val) ++count;
        if (j > 0 && midv[j-1] == val) ++count;
        if (j < 2 && midv[j+1] == val) ++count;
        if (downv[j]    == val) ++count;
        if (!(count == 1 || count == 2)) return false;
    }
    return true;
}

/* ------------------------------------------------------------------- */
/* Sequence computation                                                */
/* ------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1 */

    /* Precompute transition masks once */
    static uint32_t trans[27][27];
    static bool trans_ready = false;
    if (!trans_ready) {
        for (int up = 0; up < 27; ++up)
            for (int mid = 0; mid < 27; ++mid) {
                uint32_t mask = 0;
                for (int down = 0; down < 27; ++down)
                    if (good_row(mid, up, down))
                        mask |= (1u << down);
                trans[up][mid] = mask;
            }
        trans_ready = true;
    }

    /* DP tables */
    static seq_t dp[27][27];
    static seq_t nxt[27][27];
    for (int i = 0; i < 27; ++i)
        for (int j = 0; j < 27; ++j)
            dp[i][j] = 0;

    /* n == 1 */
    if (n == 1) {
        seq_t ans = 0;
        for (int row = 0; row < 27; ++row)
            if (good_row_only_horiz(row))
                ++ans;
        return ans / 3;          /* divide by 3 */
    }

    /* Initialize for rows 1 and 2 */
    for (int r1 = 0; r1 < 27; ++r1)
        for (int r2 = 0; r2 < 27; ++r2)
            if (good_row_only_down(r1, r2))
                dp[r1][r2] = 1;

    /* n == 2 */
    if (n == 2) {
        seq_t ans = 0;
        for (int up = 0; up < 27; ++up)
            for (int cur = 0; cur < 27; ++cur)
                if (dp[up][cur] && good_row_no_down(cur, up))
                    ans += dp[up][cur];
        return ans / 3;
    }

    /* Process rows 3..n */
    for (idx_t r = 3; r <= n; ++r) {
        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                nxt[i][j] = 0;

        for (int up = 0; up < 27; ++up)
            for (int cur = 0; cur < 27; ++cur) {
                seq_t ways = dp[up][cur];
                if (!ways) continue;
                uint32_t mask = trans[up][cur];
                while (mask) {
                    int nxtver = __builtin_ctz(mask);
                    mask &= mask - 1;
                    nxt[cur][nxtver] += ways;
                }
            }

        for (int i = 0; i < 27; ++i)
            for (int j = 0; j < 27; ++j)
                dp[i][j] = nxt[i][j];
    }

    /* Final check for the last row */
    seq_t ans = 0;
    for (int up = 0; up < 27; ++up)
        for (int cur = 0; cur < 27; ++cur)
            if (dp[up][cur] && good_row_no_down(cur, up))
                ans += dp[up][cur];

    return ans / 3;
}
