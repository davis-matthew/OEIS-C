/*
 * A237309  Number of (n+1)X(1+1) 0..3 arrays with the maximum plus the lower
 *          median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A237309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{s=0}^6 ∑_{i,j} (A_s^n)[i][j] , where A_s[i][j] = 1
 *            iff the 2×2 sub‑block formed by the two rows i and j has max +
 *            lower median – min == s. The DP below counts the walks of length
 *            n in each graph A_s
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* adjacency: adj[s][i][j] == 1  ⇔  expr(row i,row j) == s */
    static uint8_t adj[7][16][16] = {0};
    static bool initialized = false;

    if (!initialized) {
        /* pre‑compute adjacency matrix for all s in 0..6 */
        uint8_t first[16];
        uint8_t second[16];
        for (int i = 0; i < 16; ++i) {
            first[i]  = i / 4;
            second[i] = i % 4;
        }

        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int a = first[i];
                int b = second[i];
                int c = first[j];
                int d = second[j];

                /* max and min of the four numbers */
                int maxv = a;
                if (b > maxv) maxv = b;
                if (c > maxv) maxv = c;
                if (d > maxv) maxv = d;

                int minv = a;
                if (b < minv) minv = b;
                if (c < minv) minv = c;
                if (d < minv) minv = d;

                /* lower median of four numbers */
                int t0 = a, t1 = b, t2 = c, t3 = d;
                if (t0 > t1) { int tmp = t0; t0 = t1; t1 = tmp; }
                if (t2 > t3) { int tmp = t2; t2 = t3; t3 = tmp; }
                if (t0 > t2) { int tmp = t0; t0 = t2; t2 = tmp; }
                if (t1 > t3) { int tmp = t1; t1 = t3; t3 = tmp; }
                if (t1 > t2) { int tmp = t1; t1 = t2; t2 = tmp; }
                int low = t1;

                int s = maxv + low - minv;   /* expression value */
                if (s >= 0 && s <= 6) {
                    adj[s][i][j] = 1;
                }
            }
        }
        initialized = true;
    }

    seq_t total = 0;
    seq_t dp_prev[16];
    seq_t dp_cur[16];

    for (int s = 0; s <= 6; ++s) {
        /* starting with any first row */
        for (int i = 0; i < 16; ++i) dp_prev[i] = 1;

        /* walk through n steps (n sub‑blocks) */
        for (idx_t step = 1; step <= n; ++step) {
            for (int j = 0; j < 16; ++j) dp_cur[j] = 0;
            for (int i = 0; i < 16; ++i) {
                seq_t val = dp_prev[i];
                if (!val) continue;
                for (int j = 0; j < 16; ++j) {
                    if (adj[s][i][j]) dp_cur[j] += val;
                }
            }
            for (int i = 0; i < 16; ++i) dp_prev[i] = dp_cur[i];
        }

        /* sum over all possible last rows */
        seq_t sum_s = 0;
        for (int i = 0; i < 16; ++i) sum_s += dp_prev[i];
        total += sum_s;
    }

    return total;
}
