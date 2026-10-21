/*
 * A200804  Number of nX3 0..2 arrays with no average of any element and its
 *          diagonal, antidiagonal, horizontal and vertical neighbors equal to
 *          one.
 * https://oeis.org/A200804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×3 0..2 arrays with no neighbourhood‑average
 *            equal to 1 (computed by a dynamic‑programming automaton with
 *            state of the previous two rows)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define ROWS 27

/* helper: test whether a middle row satisfies the constraint with optional
   upper and lower neighbours. `up` or `down` can be NULL to indicate the
   absence of that row. */
static inline bool check_row(const int *mid, const int *up, const int *down)
{
    for (int j = 0; j < 3; ++j) {
        int sum  = mid[j];
        int count = 1;
        /* horizontal neighbours */
        if (j > 0) { sum += mid[j-1]; ++count; }
        if (j < 2) { sum += mid[j+1]; ++count; }
        /* vertical neighbours and diagonals */
        if (up) {
            sum += up[j]; ++count;
            if (j > 0) { sum += up[j-1]; ++count; }
            if (j < 2) { sum += up[j+1]; ++count; }
        }
        if (down) {
            sum += down[j]; ++count;
            if (j > 0) { sum += down[j-1]; ++count; }
            if (j < 2) { sum += down[j+1]; ++count; }
        }
        if (sum == count) return false;              /* forbidden average */
    }
    return true;
}

seq_t a(idx_t n)
{
    /* pre‑compute all row patterns (0..26) as 3‑digit base‑3 numbers */
    int row[ROWS][3];
    for (int i = 0; i < ROWS; ++i) {
        int x = i;
        row[i][0] = x % 3; x /= 3;
        row[i][1] = x % 3; x /= 3;
        row[i][2] = x % 3;
    }

    /* validity tables */
    static unsigned char full_valid[ROWS];
    static unsigned char top_valid[ROWS][ROWS];
    static unsigned char bottom_valid[ROWS][ROWS];
    static unsigned char mid_valid[ROWS][ROWS][ROWS];
    static unsigned char trans_list[ROWS][ROWS][ROWS];
    static unsigned char trans_len[ROWS][ROWS];

    /* compute full_valid */
    for (int a = 0; a < ROWS; ++a)
        full_valid[a] = check_row(row[a], NULL, NULL) ? 1 : 0;

    /* compute top_valid (up missing, down present) */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            top_valid[a][b] = check_row(row[a], NULL, row[b]) ? 1 : 0;

    /* compute bottom_valid (up present, down missing) */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            bottom_valid[a][b] = check_row(row[b], row[a], NULL) ? 1 : 0;

    /* compute mid_valid and transition lists */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b) {
            unsigned char idx = 0;
            for (int c = 0; c < ROWS; ++c) {
                unsigned char ok = check_row(row[b], row[a], row[c]) ? 1 : 0;
                mid_valid[a][b][c] = ok;
                if (ok)
                    trans_list[a][b][idx++] = (unsigned char)c;
            }
            trans_len[a][b] = idx;
        }

    /* DP arrays */
    static seq_t dp[ROWS][ROWS];
    static seq_t dp_next[ROWS][ROWS];
    memset(dp, 0, sizeof(dp));

    if (n == 1) {
        seq_t result = 0;
        for (int a = 0; a < ROWS; ++a)
            if (full_valid[a]) result += 1;
        return result;
    }

    /* initial step: first two rows */
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            if (top_valid[a][b]) dp[a][b] = 1;

    /* process remaining rows */
    for (idx_t rowi = 2; rowi < n; ++rowi) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                seq_t cur_cnt = dp[a][b];
                if (!cur_cnt) continue;
                unsigned char len = trans_len[a][b];
                for (unsigned char k = 0; k < len; ++k) {
                    unsigned char c = trans_list[a][b][k];
                    dp_next[b][c] += cur_cnt;
                }
            }
        }
        memcpy(dp, dp_next, sizeof(dp));
    }

    /* finally validate the last row */
    seq_t result = 0;
    for (int a = 0; a < ROWS; ++a)
        for (int b = 0; b < ROWS; ++b)
            if (bottom_valid[a][b]) result += dp[a][b];

    return result;
}
