/*
 * A223871  Number of n X 3 0..3 arrays with rows, diagonals and antidiagonals
 *          unimodal.
 * https://oeis.org/A223871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n X 3 0..3 arrays with rows, diagonals and
 *            antidiagonals unimodal. The computation uses a
 *            dynamic‑programming over row patterns
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   unimodal3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/unimodal3.h"

/* maximum number of possible row patterns (0..3)^3 = 64 */
#define ROWS 64

/* static tables initialised once */
static bool init_done = false;
static bool row_unimodal[ROWS];
static uint8_t row_vals[ROWS][3];
static uint8_t next_cnt[ROWS][ROWS];
static uint8_t next_tbl[ROWS][ROWS][ROWS];   /* next state list */

/* number of unimodal rows (pre‑computed) */
static unsigned long long unimodal_row_count = 0;

seq_t a(idx_t n)
{
    /* ----------  initialise tables on first call  ---------- */
    if (!init_done) {
        /* build row value tables and unimodality flags */
        for (int r = 0; r < ROWS; ++r) {
            row_vals[r][0] = (r >> 4) & 3;
            row_vals[r][1] = (r >> 2) & 3;
            row_vals[r][2] = r & 3;
            bool ok = unimodal3(row_vals[r][0], row_vals[r][1], row_vals[r][2]);
            row_unimodal[r] = ok;
            if (ok) ++unimodal_row_count;
        }

        /* pre‑compute allowed transitions */
        for (int r1 = 0; r1 < ROWS; ++r1) {
            for (int r2 = 0; r2 < ROWS; ++r2) {
                if (!row_unimodal[r1] || !row_unimodal[r2]) {
                    next_cnt[r1][r2] = 0;
                    continue;
                }
                unsigned int cnt = 0;
                for (int r3 = 0; r3 < ROWS; ++r3) {
                    if (!row_unimodal[r3]) continue;
                    /* NW‑SE diagonal triple */
                    if (!unimodal3(row_vals[r1][0], row_vals[r2][1], row_vals[r3][2]))
                        continue;
                    /* NE‑SW (antidiagonal) triple */
                    if (!unimodal3(row_vals[r1][2], row_vals[r2][1], row_vals[r3][0]))
                        continue;
                    next_tbl[r1][r2][cnt++] = (uint8_t)r3;
                }
                next_cnt[r1][r2] = (uint8_t)cnt;
            }
        }
        init_done = true;
    }

    /* ----------  trivial case: n == 1  ---------- */
    if (n == 1) {
        return (seq_t)unimodal_row_count;
    }

    /* ----------  DP tables for 64×64 states  ---------- */
    static seq_t dp[ROWS][ROWS];
    static seq_t dp2[ROWS][ROWS];
    /* initialise dp with all pairs of unimodal rows (arrays of length 2) */
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < ROWS; ++j) {
            dp[i][j] = 0;
        }
    }
    const seq_t one = 1;
    for (int r1 = 0; r1 < ROWS; ++r1) {
        if (!row_unimodal[r1]) continue;
        for (int r2 = 0; r2 < ROWS; ++r2) {
            if (!row_unimodal[r2]) continue;
            dp[r1][r2] = one;
        }
    }

    /* ----------  DP transition for n >= 3  ---------- */
    for (idx_t step = 3; step <= n; ++step) {
        /* zero dp2 */
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                dp2[i][j] = 0;
            }
        }
        /* perform transition */
        for (int a = 0; a < ROWS; ++a) {
            for (int b = 0; b < ROWS; ++b) {
                seq_t cur = dp[a][b];
                if (cur == 0) continue;
                unsigned int cnt = (unsigned int)next_cnt[a][b];
                const uint8_t *list = next_tbl[a][b];
                for (unsigned int t = 0; t < cnt; ++t) {
                    int r3 = list[t];
                    dp2[b][r3] += cur;
                }
            }
        }
        /* copy dp2 into dp for next iteration */
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < ROWS; ++j) {
                dp[i][j] = dp2[i][j];
            }
        }
    }

    /* ----------  sum all states to get final result  ---------- */
    seq_t total = 0;
    for (int a = 0; a < ROWS; ++a) {
        for (int b = 0; b < ROWS; ++b) {
            total += dp[a][b];
        }
    }
    return total;
}
