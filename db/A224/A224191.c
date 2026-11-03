/*
 * A224191  Number of 4Xn 0..2 arrays with rows unimodal and columns
 *          nondecreasing.
 * https://oeis.org/A224191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of 4×n arrays with entries 0..2, columns
 *            non‑decreasing, rows unimodal
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 14, u64 n <= 73, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 205 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Helper to build all admissible columns (non‑decreasing 4‑tuples). */
static void _build_columns(int col[15][4])
{
    int k = 0;
    for (int a = 0; a <= 2; ++a)
        for (int b = a; b <= 2; ++b)
            for (int c = b; c <= 2; ++c)
                for (int d = c; d <= 2; ++d) {
                    col[k][0] = a;
                    col[k][1] = b;
                    col[k][2] = c;
                    col[k][3] = d;
                    ++k;
                }
}

/* Build transition table.
   For each state s and each admissible column t, compute the next state ns.
   States: 0..8 encode prior values (0..2), 9..17 encode value 1 with trend 1, etc.
   State encoding: s = e0 + 6*e1 + 36*e2 + 216*e3  (e0 is least significant). */
static void _build_trans(const int col[15][4], int16_t trans[1296][15])
{
    for (int s = 0; s < 1296; ++s) {
        int rem = s;
        int prev_val[4];
        int inc[4];
        for (int r = 0; r < 4; ++r) {
            int enc = rem % 6;   /* enc = value*2 + inc_bit */
            rem /= 6;
            prev_val[r] = enc >> 1; /* value part */
            inc[r] = enc & 1;       /* inc bit: 1 = still increasing, 0 = decreasing */
        }
        for (int t = 0; t < 15; ++t) {
            int dest_enc[4];
            bool ok = true;
            for (int r = 0; r < 4; ++r) {
                int nv = col[t][r];
                if (inc[r]) {                /* still increasing */
                    if (nv >= prev_val[r])
                        dest_enc[r] = (nv << 1) | 1;   /* stay increasing */
                    else
                        dest_enc[r] = (nv << 1) | 0;   /* start decreasing */
                } else {                     /* already decreasing */
                    if (nv <= prev_val[r])
                        dest_enc[r] = (nv << 1) | 0;   /* stay decreasing */
                    else {
                        ok = false;
                        break;
                    }
                }
            }
            if (!ok) {
                trans[s][t] = -1;
            } else {
                int ns = dest_enc[0] + 6*dest_enc[1] + 36*dest_enc[2] + 216*dest_enc[3];
                trans[s][t] = (int16_t)ns;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    static int col[15][4];
    static int16_t trans[1296][15];
    static bool built = false;

    if (!built) {
        _build_columns(col);
        _build_trans(col, trans);
        built = true;
    }

    /* dp[s] = number of ways to reach state s after current column */
    seq_t dp[1296] = {0};
    seq_t dp_next[1296];

    /* initial column: every admissible column can be first */
    for (int t = 0; t < 15; ++t) {
        int s = ((col[t][0] << 1) | 1) + 6*((col[t][1] << 1) | 1)
              + 36*((col[t][2] << 1) | 1) + 216*((col[t][3] << 1) | 1);
        dp[s] = dp[s] + 1;
    }

    /* process remaining columns */
    for (idx_t step = 1; step < n; ++step) {
        for (int i = 0; i < 1296; ++i) dp_next[i] = 0;
        for (int s = 0; s < 1296; ++s) {
            seq_t ways = dp[s];
            if (ways == 0) continue;
            for (int t = 0; t < 15; ++t) {
                int ns = trans[s][t];
                if (ns >= 0)
                    dp_next[ns] = dp_next[ns] + ways;
            }
        }
        for (int i = 0; i < 1296; ++i) dp[i] = dp_next[i];
    }

    seq_t result = 0;
    for (int s = 0; s < 1296; ++s) result = result + dp[s];
    return result;
}
