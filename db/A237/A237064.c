/*
 * A237064  Number of (n+1)X(5+1) 0..2 arrays with the maximum plus the upper
 *          median plus the minimum minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×6 arrays of 0,1,2 such that for each 2×2
 *            subgrid the value max + upper_median + min − lower_median is
 *            equal for all subgrids
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- initialise data structures only once ---------- */
    static bool init = false;
    static unsigned short row_vals[729][6];
    static unsigned short next_rows[729][5][729];
    static unsigned short next_cnt[729][5];
    static seq_t dp_cur[729][5];
    static seq_t dp_next[729][5];

    if (!init) {
        /* enumerate all 729 rows of length 6 with entries 0–2 */
        for (int id = 0; id < 729; ++id) {
            int t = id;
            for (int j = 5; j >= 0; --j) {
                row_vals[id][j] = t % 3;
                t /= 3;
            }
        }

        /* compute transition lists: for each pair (r1,r2) find constant C,
           if it exists, and record r2 as a successor of r1 with that C */
        for (int r1 = 0; r1 < 729; ++r1) {
            for (int c = 0; c < 5; ++c)
                next_cnt[r1][c] = 0;

            for (int r2 = 0; r2 < 729; ++r2) {
                int C = -1;
                bool ok = true;

                for (int j = 0; j < 5; ++j) {
                    int v[4];
                    v[0] = row_vals[r1][j];
                    v[1] = row_vals[r1][j + 1];
                    v[2] = row_vals[r2][j];
                    v[3] = row_vals[r2][j + 1];

                    /* small sorting network for 4 items */
                    if (v[0] > v[1]) { int t = v[0]; v[0] = v[1]; v[1] = t; }
                    if (v[2] > v[3]) { int t = v[2]; v[2] = v[3]; v[3] = t; }
                    if (v[0] > v[2]) { int t = v[0]; v[0] = v[2]; v[2] = t; }
                    if (v[1] > v[3]) { int t = v[1]; v[1] = v[3]; v[3] = t; }
                    if (v[1] > v[2]) { int t = v[1]; v[1] = v[2]; v[2] = t; }

                    int e = v[3] + v[2] + v[0] - v[1];
                    if (C == -1) C = e;
                    else if (e != C) { ok = false; break; }
                }

                if (ok) {
                    unsigned short *list = next_rows[r1][C];
                    list[next_cnt[r1][C]++] = r2;
                }
            }
        }
        init = true;
    }

    /* ---------- handle n==0 (offset) gracefully ---------- */
    if (n == 0) return 0;

    /* ---------- DP initialisation: all pairs (r1,r2) create length‑2 arrays ---------- */
    for (int i = 0; i < 729; ++i)
        for (int c = 0; c < 5; ++c)
            dp_cur[i][c] = 0;

    for (int r1 = 0; r1 < 729; ++r1) {
        for (int C = 0; C < 5; ++C) {
            unsigned short cnt = next_cnt[r1][C];
            if (!cnt) continue;
            unsigned short *list = next_rows[r1][C];
            for (unsigned short k = 0; k < cnt; ++k) {
                unsigned short r2 = list[k];
                dp_cur[r2][C] += (seq_t)1;
            }
        }
    }

    /* ---------- extend to (n+1) rows (i.e., n‑1 additional transitions) ---------- */
    idx_t steps = n >= 1 ? n - 1 : 0;
    for (idx_t step = 0; step < steps; ++step) {
        /* zero dp_next */
        for (int i = 0; i < 729; ++i)
            for (int c = 0; c < 5; ++c)
                dp_next[i][c] = 0;

        /* transition */
        for (int r1 = 0; r1 < 729; ++r1) {
            for (int C = 0; C < 5; ++C) {
                seq_t cur = dp_cur[r1][C];
                if (cur == 0) continue;
                unsigned short cnt = next_cnt[r1][C];
                unsigned short *list = next_rows[r1][C];
                for (unsigned short k = 0; k < cnt; ++k) {
                    unsigned short r2 = list[k];
                    dp_next[r2][C] += cur;
                }
            }
        }

        /* move dp_next to dp_cur */
        for (int i = 0; i < 729; ++i)
            for (int c = 0; c < 5; ++c)
                dp_cur[i][c] = dp_next[i][c];
    }

    /* ---------- sum all valid arrays of length n+1 ---------- */
    seq_t total = 0;
    for (int i = 0; i < 729; ++i)
        for (int c = 0; c < 5; ++c)
            total += dp_cur[i][c];

    return total;
}
