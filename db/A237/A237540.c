/*
 * A237540  Number of (n+1)X(2+1) 0..3 arrays with the maximum plus the lower
 *          median minus the upper median minus the minimum of every 2X2
 *          subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..3 such that for
 *            every 2×2 subblock the value k = max + lower_median −
 *            upper_median − min differs from its horizontal and vertical
 *            neighbors by exactly one. Computed by a transfer‑matrix DP over
 *            rows
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline int compute_k(int a, int b, int c, int d)
{
    int vals[4] = {a, b, c, d};
    /* simple insertion sort for 4 elements */
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (vals[j] < vals[i]) {
                int x = vals[i]; vals[i] = vals[j]; vals[j] = x;
            }
        }
    }
    return vals[3] + vals[1] - vals[2] - vals[0];
}

seq_t a(idx_t n)
{
    /* Pre‑computation tables (static, computed once) */
    static bool init = false;
    static uint8_t row[64][3];
    static int8_t kidx[4][4];
    static uint8_t trans_cur[64][64];
    static uint8_t trans_k0[64][64];
    static uint8_t trans_k1[64][64];
    static uint8_t trans_kindex[64][64];
    static uint8_t trans_last[64];
    static const int KVAL[6][2] = {
        {0, 1}, {1, 0}, {1, 2}, {2, 1}, {2, 3}, {3, 2}
    };

    if (!init) {
        /* row encoding: index = a*16 + b*4 + c */
        for (int i = 0; i < 64; ++i) {
            row[i][0] = i >> 4;          /* a */
            row[i][1] = (i >> 2) & 3;    /* b */
            row[i][2] = i & 3;           /* c */
        }

        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                kidx[i][j] = -1;
        kidx[0][1] = 0;
        kidx[1][0] = 1;
        kidx[1][2] = 2;
        kidx[2][1] = 3;
        kidx[2][3] = 4;
        kidx[3][2] = 5;

        for (int pr = 0; pr < 64; ++pr) {
            int count = 0;
            for (int cr = 0; cr < 64; ++cr) {
                int a0 = row[pr][0], b0 = row[pr][1], c0 = row[cr][0], d0 = row[cr][1];
                int a1 = row[pr][1], b1 = row[pr][2], c1 = row[cr][1], d1 = row[cr][2];
                int k0 = compute_k(a0, b0, c0, d0);
                int k1 = compute_k(a1, b1, c1, d1);
                int kk = kidx[k0][k1];
                if (kk >= 0) {
                    trans_cur[pr][count] = (uint8_t)cr;
                    trans_k0[pr][count] = (uint8_t)k0;
                    trans_k1[pr][count] = (uint8_t)k1;
                    trans_kindex[pr][count] = (uint8_t)kk;
                    ++count;
                }
            }
            trans_last[pr] = count;
        }
        init = true;
    }

    /* DP arrays */
    static seq_t dp0[64][6];
    static seq_t dp1[64][6];

    /* reset dp0 for base computation */
    for (int i = 0; i < 64; ++i)
        for (int j = 0; j < 6; ++j)
            dp0[i][j] = (seq_t)0;

    /* Base: all valid pairs of first two rows */
    for (int r0 = 0; r0 < 64; ++r0) {
        for (int t = 0; t < trans_last[r0]; ++t) {
            uint8_t cr = trans_cur[r0][t];
            uint8_t ki = trans_kindex[r0][t];
            dp0[cr][ki] = (seq_t)(dp0[cr][ki] + (seq_t)1);
        }
    }

    if (n == (idx_t)1) {
        seq_t ans = (seq_t)0;
        for (int r = 0; r < 64; ++r)
            for (int k = 0; k < 6; ++k)
                ans += dp0[r][k];
        return ans;
    }

    seq_t (*prev)[6] = dp0;
    seq_t (*cur)[6]  = dp1;

    for (idx_t row_idx = 2; row_idx <= n; ++row_idx) {
        /* clear current DP */
        for (int i = 0; i < 64; ++i)
            for (int j = 0; j < 6; ++j)
                cur[i][j] = (seq_t)0;

        for (int pr = 0; pr < 64; ++pr) {
            for (int kprev = 0; kprev < 6; ++kprev) {
                seq_t val = prev[pr][kprev];
                if (!val) continue;
                int k0prev = KVAL[kprev][0];
                int k1prev = KVAL[kprev][1];
                int tcnt   = trans_last[pr];
                for (int t = 0; t < tcnt; ++t) {
                    int c = trans_k0[pr][t];
                    int d = trans_k1[pr][t];
                    if ((k0prev > c ? k0prev - c : c - k0prev) != 1) continue;
                    if ((k1prev > d ? k1prev - d : d - k1prev) != 1) continue;
                    uint8_t cr      = trans_cur[pr][t];
                    uint8_t ki      = trans_kindex[pr][t];
                    cur[cr][ki] = (seq_t)(cur[cr][ki] + val);
                }
            }
        }
        /* swap DP layers */
        seq_t (*tmp)[6] = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t ans = (seq_t)0;
    for (int r = 0; r < 64; ++r)
        for (int k = 0; k < 6; ++k)
            ans += prev[r][k];
    return ans;
}
