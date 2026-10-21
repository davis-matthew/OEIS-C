/*
 * A238318  Number of (n+1) X (3+1) 0..2 arrays with no element greater than
 *          all horizontal neighbors or less than all vertical neighbors.
 * https://oeis.org/A238318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of (n+1)×4 arrays over {0,1,2} in which
 *  • for each row, no element is greater than all its horizontal neighbors
 *    (rows endpoints must not exceed their single horizontal neighbour; 
 *   middle cells must not exceed both neighbours);
 *  • for each column, no element is less than both its vertical neighbors
 *    (at the first and last rows the element must be ≥ its single vertical
 *    neighbour).
 * The function uses a transfer‑matrix dynamic programming in O(n·R²)
 * where R is the number of rows that satisfy the horizontal constraints
 * (R=22 for this problem).
 */
 
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
 
seq_t a(idx_t n)
{
    /* ---------- precomputation ---------- */
    static bool ready = false;
    static bool valid_row[81];
    static int  row_val[81][4];
    static bool ge[81][81];              /* a >= b element‑wise */
    static unsigned char next_list[81][81][81];
    static uint8_t next_len[81][81];
    static seq_t dp_cur[81][81];
    static seq_t dp_next[81][81];
 
    if (!ready) {
        /* encode all 81 rows (base‑3 representation) */
        for (int s = 0; s < 81; ++s) {
            int t = s;
            for (int j = 0; j < 4; ++j) {
                row_val[s][j] = t % 3;
                t /= 3;
            }
        }
        /* horizontal validity: not greater than all horizontal neighbours */
        for (int s = 0; s < 81; ++s) {
            bool ok = true;
            for (int j = 0; j < 4 && ok; ++j) {
                int left  = (j > 0)   ? row_val[s][j-1] : -1;
                int right = (j < 3)   ? row_val[s][j+1] : -1;
                if (left < 0) {               /* leftmost */
                    if (row_val[s][j] > right) ok = false;
                } else if (right < 0) {        /* rightmost */
                    if (row_val[s][j] > left)  ok = false;
                } else {                       /* middle */
                    if (row_val[s][j] > left && row_val[s][j] > right)
                        ok = false;
                }
            }
            valid_row[s] = ok;
        }
        /* element‑wise greater‑than-or‑equal matrix */
        for (int a = 0; a < 81; ++a)
            for (int b = 0; b < 81; ++b) {
                bool ok = true;
                for (int j = 0; j < 4; ++j)
                    if (row_val[a][j] < row_val[b][j]) {
                        ok = false; break;
                    }
                ge[a][b] = ok;
            }
        /* adjacency list for vertical middle constraint:
           for each pair (a,b) store all c such that b ≥ min(a,c) element‑wise
           (i.e. not (b < a && b < c) at any column) */
        for (int a = 0; a < 81; ++a)
            for (int b = 0; b < 81; ++b) {
                if (!valid_row[a] || !valid_row[b]) { next_len[a][b] = 0; continue; }
                uint8_t cnt = 0;
                for (int c = 0; c < 81; ++c) {
                    if (!valid_row[c]) continue;
                    bool ok = true;
                    for (int j = 0; j < 4; ++j)
                        if (row_val[b][j] < row_val[a][j] &&
                            row_val[b][j] < row_val[c][j]) {
                            ok = false; break;
                        }
                    if (ok) next_list[a][b][cnt++] = (unsigned char)c;
                }
                next_len[a][b] = cnt;
            }
        ready = true;
    }
 
    /* ---------- dynamic programming ---------- */
    idx_t rows = n + 1;                 /* number of rows */
    if (rows < 2) return 0;             /* undefined for n = 0 */
 
    /* zero current DP table */
    for (int i = 0; i < 81; ++i)
        for (int j = 0; j < 81; ++j)
            dp_cur[i][j] = 0;
    /* initialise first two rows: top >= second (element‑wise) */
    for (int a = 0; a < 81; ++a)
        if (valid_row[a])
            for (int b = 0; b < 81; ++b)
                if (valid_row[b] && ge[a][b])
                    dp_cur[a][b] = (seq_t)1;
 
    /* process remaining rows */
    for (idx_t step = 0; step < rows - 2; ++step) {
        /* zero next DP table */
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                dp_next[i][j] = 0;
        /* transition */
        for (int a = 0; a < 81; ++a) {
            for (int b = 0; b < 81; ++b) {
                seq_t cur_cnt = dp_cur[a][b];
                if (!cur_cnt) continue;
                uint8_t cnt = next_len[a][b];
                for (uint8_t k = 0; k < cnt; ++k) {
                    int c = next_list[a][b][k];
                    dp_next[b][c] += cur_cnt;
                }
            }
        }
        /* copy next to current */
        for (int i = 0; i < 81; ++i)
            for (int j = 0; j < 81; ++j)
                dp_cur[i][j] = dp_next[i][j];
    }
 
    /* apply bottom constraint: last row >= previous row (element‑wise) */
    seq_t total = 0;
    for (int a = 0; a < 81; ++a)
        for (int b = 0; b < 81; ++b)
            if (dp_cur[a][b] && ge[b][a])
                total += dp_cur[a][b];
    return total;
}
