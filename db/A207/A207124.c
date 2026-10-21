/*
 * A207124  Number of 4Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A207124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A207124: Number of 4×n binary arrays avoiding the patterns
   0 0 1 and 0 1 0 horizontally and 0 0 1 and 0 1 1 vertically.
   Computed by dynamic programming over columns. */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static bool vertical_ok[16];
static int  valid_vals[16];
static int  valid_cnt;

/* initialise tables once */
static void init_tables(void)
{
    if (valid_cnt != 0) return; /* already initialised */
    for (int c = 0; c < 16; ++c) {
        int b0 = (c >> 0) & 1;
        int b1 = (c >> 1) & 1;
        int b2 = (c >> 2) & 1;
        int b3 = (c >> 3) & 1;
        bool ok = true;
        /* rows 0–2 */
        if ((b0 == 0 && b1 == 0 && b2 == 1) || (b0 == 0 && b1 == 1 && b2 == 1))
            ok = false;
        /* rows 1–3 */
        if ((b1 == 0 && b2 == 0 && b3 == 1) || (b1 == 0 && b2 == 1 && b3 == 1))
            ok = false;
        vertical_ok[c] = ok;
        if (ok) {
            valid_vals[valid_cnt++] = c;
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();
    /* n == 0 is outside the offset; return 0 */
    if (n == 0) return (seq_t)0;

    /* n == 1: each column independently */
    if (n == 1) {
        return (seq_t)valid_cnt;
    }

    /* n == 2: all pairs of valid columns are allowed */
    if (n == 2) {
        return (seq_t)valid_cnt * (seq_t)valid_cnt;
    }

    /* DP over columns: dp_cur[pp][p] = number of arrays ending with
       columns pp (second‑last) and p (last) after processing current
       number of columns.  Only indices that correspond to valid columns
       need to be considered.  dp_cur is indexed by full 4‑bit values
       0…15.  Unused entries stay zero. */
    static seq_t dp_cur[16][16];
    static seq_t dp_next[16][16];

    /* initialise dp_cur for 2 columns - each pair contributes 1 */
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            dp_cur[i][j] = (seq_t)0;
        }
    }
    for (int ii = 0; ii < valid_cnt; ++ii) {
        int pp = valid_vals[ii];
        for (int jj = 0; jj < valid_cnt; ++jj) {
            int p = valid_vals[jj];
            dp_cur[pp][p] = (seq_t)1;
        }
    }

    /* process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        /* zero dp_next */
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                dp_next[i][j] = (seq_t)0;
            }
        }

        /* transition */
        for (int ii = 0; ii < valid_cnt; ++ii) {
            int pp = valid_vals[ii];
            for (int jj = 0; jj < valid_cnt; ++jj) {
                int p = valid_vals[jj];
                seq_t cur_val = dp_cur[pp][p];
                if (cur_val == (seq_t)0) continue;
                for (int kk = 0; kk < valid_cnt; ++kk) {
                    int c = valid_vals[kk];
                    /* check horizontal constraints on all 4 rows */
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        int a = (pp >> r) & 1;
                        int b = (p  >> r) & 1;
                        int d = (c  >> r) & 1;
                        if ((a == 0 && b == 0 && d == 1) ||
                            (a == 0 && b == 1 && d == 0)) {
                            ok = false;
                            break;
                        }
                    }
                    if (ok) {
                        dp_next[p][c] += cur_val;
                    }
                }
            }
        }

        /* swap dp_cur and dp_next */
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                dp_cur[i][j] = dp_next[i][j];
            }
        }
    }

    /* sum over all final states */
    seq_t result = (seq_t)0;
    for (int ii = 0; ii < valid_cnt; ++ii) {
        int pp = valid_vals[ii];
        for (int jj = 0; jj < valid_cnt; ++jj) {
            int p = valid_vals[jj];
            result += dp_cur[pp][p];
        }
    }
    return result;
}
