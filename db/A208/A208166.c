/*
 * A208166  Number of 5Xn 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 0 1 0 vertically.
 * https://oeis.org/A208166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays avoiding the patterns 0 0 0
 *            and 1 0 1 horizontally and 0 0 1 and 0 1 0 vertically. Computed
 *            via dynamic programming on column states (size 5)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Enumerate all 32 possible 5-bit column vectors (rows 0‑4). */
    const int COLS = 32;
    int valid[COLS];
    int M = 0;  /* count of valid columns */

    for (int i = 0; i < COLS; ++i) {
        int mask = i;
        int ok = 1;
        for (int r = 0; r <= 2; ++r) {          /* check vertical triples */
            int a = (mask >> r) & 1;
            int b = (mask >> (r + 1)) & 1;
            int c = (mask >> (r + 2)) & 1;
            if (a == 0 && b == 0 && c == 1) { ok = 0; break; }
            if (a == 0 && b == 1 && c == 0) { ok = 0; break; }
        }
        if (ok) valid[M++] = i;
    }

    if (n == 0) return 0;                     /* undefined, but harmless */
    if (n == 1) return (seq_t)M;
    if (n == 2) return (seq_t)M * (seq_t)M;

    /* Map valid columns to a compact index. */
    const int MAX_M = 14;                     /* we know from the pattern that M ≤ 14 */
    int colidx[MAX_M];
    for (int i = 0; i < M; ++i) colidx[i] = valid[i];

    /* Precompute allowed transitions: from pair (a,b) to column c. */
    uint8_t trans_cnt[MAX_M][MAX_M];
    int trans_list[MAX_M][MAX_M][MAX_M];
    for (int a = 0; a < M; ++a) {
        for (int b = 0; b < M; ++b) {
            uint8_t cnt = 0;
            int ma = colidx[a];
            int mb = colidx[b];
            for (int c = 0; c < M; ++c) {
                int mc = colidx[c];
                int ok = 1;
                for (int r = 0; r < 5; ++r) {            /* check horizontal triples */
                    int va = (ma >> r) & 1;
                    int vb = (mb >> r) & 1;
                    int vc = (mc >> r) & 1;
                    if (va == 0 && vb == 0 && vc == 0) { ok = 0; break; }
                    if (va == 1 && vb == 0 && vc == 1) { ok = 0; break; }
                }
                if (ok) trans_list[a][b][cnt++] = c;
            }
            trans_cnt[a][b] = cnt;
        }
    }

    const int MAP = MAX_M * MAX_M;
    seq_t dp_cur[MAP];
    seq_t dp_next[MAP];

    /* Initial DP for 2 columns: each pair is a valid configuration. */
    for (int i = 0; i < MAP; ++i) dp_cur[i] = (seq_t)1;

    /* Iterate through columns 3..n */
    for (idx_t step = 3; step <= n; ++step) {
        for (int i = 0; i < MAP; ++i) dp_next[i] = (seq_t)0;
        for (int a = 0; a < M; ++a) {
            for (int b = 0; b < M; ++b) {
                seq_t val = dp_cur[a * MAX_M + b];
                if (val == (seq_t)0) continue;
                uint8_t cnt = trans_cnt[a][b];
                for (uint8_t t = 0; t < cnt; ++t) {
                    int c = trans_list[a][b][t];
                    dp_next[b * MAX_M + c] += val;
                }
            }
        }
        for (int i = 0; i < MAP; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all configurations of length n. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < MAP; ++i) result += dp_cur[i];
    return result;
}
