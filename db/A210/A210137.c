/*
 * A210137  Number of (n+1)X3 0..3 arrays containing all values 0..3 with
 *          every 2X2 subblock having one, two or four distinct values, and
 *          new values 0..3 introduced in row major order.
 * https://oeis.org/A210137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2,3} that contain all
 *            four symbols, have every 2×2 subblock with 1, 2 or 4 distinct
 *            values, and introduce new symbols in row‑major order
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   build_tables
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/build_tables.h"

/* global static tables (to satisfy compiler when building in C++) */
static unsigned char _row_vals[64][3];
static unsigned char _adjacency[64][64];
static unsigned char _trans_nei[5][64][64];
static unsigned char _trans_newk[5][64][64];
static unsigned char _trans_cnt[5][64];

static void init_globals(void) {
    static int init = 0;
    if (init) return;
    init = 1;

    /* row values table */
    for (int r = 0; r < 64; ++r) {
        _row_vals[r][0] = (r >> 4) & 3;
        _row_vals[r][1] = (r >> 2) & 3;
        _row_vals[r][2] = r & 3;
    }

    /* adjacency */
    for (int p = 0; p < 64; ++p) {
        for (int r = 0; r < 64; ++r) {
            unsigned char ok = 1;
            for (int j = 0; j < 2; ++j) {
                unsigned mask =
                    (1 << _row_vals[p][j]) |
                    (1 << _row_vals[p][j+1]) |
                    (1 << _row_vals[r][j]) |
                    (1 << _row_vals[r][j+1]);
                unsigned cnt = __builtin_popcount(mask);
                if (cnt == 3) { ok = 0; break; }
            }
            _adjacency[p][r] = ok;
        }
    }

    /* transitions */
    for (int k = 0; k <= 4; ++k) {
        for (int p = 0; p < 64; ++p) {
            unsigned cnt = 0;
            for (int r = 0; r < 64; ++r) {
                if (!_adjacency[p][r]) continue;
                int k_cur = k;
                int ok = 1;
                for (int j = 0; j < 3; ++j) {
                    int v = _row_vals[r][j];
                    if (v < k_cur) continue;
                    if (v == k_cur) { k_cur++; continue; }
                    ok = 0; break;
                }
                if (!ok) continue;
                _trans_nei[k][p][cnt]   = (unsigned char)r;
                _trans_newk[k][p][cnt] = (unsigned char)k_cur;
                ++cnt;
            }
            _trans_cnt[k][p] = cnt;
        }
    }
}

/* The required function signature */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* out of bounds, return 0 */

    init_globals();

    /* dp arrays: dp[k][row] */
    static seq_t dp_prev[5][64];
    static seq_t dp_next[5][64];
    memset(dp_prev, 0, sizeof(dp_prev));
    /* initialize first row (row index 0) */
    for (int r = 0; r < 64; ++r) {
        int k = 0;
        int k_cur = k;
        int ok = 1;
        for (int j = 0; j < 3; ++j) {
            int v = _row_vals[r][j];
            if (v < k_cur) continue;
            if (v == k_cur) { k_cur++; continue; }
            ok = 0; break;
        }
        if (!ok) continue;
        dp_prev[k_cur][r] = 1;
    }

    /* process remaining n rows (total rows = n+1) */
    for (idx_t step = 1; step <= n; ++step) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int k_prev = 0; k_prev <= 4; ++k_prev) {
            for (int p = 0; p < 64; ++p) {
                seq_t val = dp_prev[k_prev][p];
                if (!val) continue;
                unsigned cnt = _trans_cnt[k_prev][p];
                for (unsigned t = 0; t < cnt; ++t) {
                    int r   = _trans_nei[k_prev][p][t];
                    int k_new = _trans_newk[k_prev][p][t];
                    dp_next[k_new][r] += val;
                }
            }
        }
        /* swap dp_next into dp_prev */
        memcpy(dp_prev, dp_next, sizeof(dp_prev));
    }

    /* sum over final states with all 4 values used (k = 4) */
    seq_t result = 0;
    for (int r = 0; r < 64; ++r) {
        result += dp_prev[4][r];
    }
    return result;
}
