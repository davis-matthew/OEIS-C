/*
 * A203538  Number of nX4 0..2 arrays with every element neighboring
 *          horizontally or vertically both a 0 and a 1.
 * https://oeis.org/A203538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A203538: number of n×4 arrays over {0,1,2}
   so that every element has neighboring horizontally or vertically
   both a 0 and a 1.  The algorithm uses dynamic programming over
   pairs of consecutive rows; it runs in O(n·81²).  */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Helper tables */
static unsigned val[81][4];           /* digits of a row in base‑3 */
static unsigned char first_cnt[81];
static unsigned short first_arr[81][81];
static unsigned char compat_cnt[81 * 81];
static unsigned short compat_arr[81 * 81][81];
static unsigned char last_ok[81][81];
static unsigned char single_ok[81];
static bool tables_ready = false;

/* Build val table */
static void init_val(void) {
    for (int idx = 0; idx < 81; ++idx) {
        int tmp = idx;
        for (int c = 0; c < 4; ++c) {
            val[idx][c] = tmp % 3;
            tmp /= 3;
        }
    }
}

/* Check that for a cell in row `row_idx` and column `col`,
   among its existing neighbors there is at least one 0
   and at least one 1.  Missing neighbors are indicated by -1. */
static inline bool cell_ok(int row_idx, int up_idx, int down_idx, int col) {
    bool h0 = false, h1 = false;

    /* left neighbor */
    if (col > 0) {
        unsigned v = val[row_idx][col - 1];
        if (v == 0) h0 = true; else if (v == 1) h1 = true;
    }
    /* right neighbor */
    if (col < 3) {
        unsigned v = val[row_idx][col + 1];
        if (v == 0) h0 = true; else if (v == 1) h1 = true;
    }
    /* up neighbor */
    if (up_idx != -1) {
        unsigned v = val[up_idx][col];
        if (v == 0) h0 = true; else if (v == 1) h1 = true;
    }
    /* down neighbor */
    if (down_idx != -1) {
        unsigned v = val[down_idx][col];
        if (v == 0) h0 = true; else if (v == 1) h1 = true;
    }
    return h0 && h1;
}

/* Row0_ok: check all cells of row 'first_row' when the only neighboring
   row is 'next_row' (down neighbor). */
static inline bool row0_ok(int first_row, int next_row) {
    for (int c = 0; c < 4; ++c) {
        if (!cell_ok(first_row, -1, next_row, c))
            return false;
    }
    return true;
}

/* last_row_ok: check all cells of row 'cur_row' when the only neighboring
   row is 'prev_row' (up). */
static inline bool last_row_ok(int prev_row, int cur_row) {
    for (int c = 0; c < 4; ++c) {
        if (!cell_ok(cur_row, prev_row, -1, c))
            return false;
    }
    return true;
}

/* single_row_check: row with no vertical neighbors, only left/right. */
static inline bool single_row_check(int row) {
    for (int c = 0; c < 4; ++c) {
        bool h0 = false, h1 = false;
        if (c > 0) {
            unsigned v = val[row][c - 1];
            if (v == 0) h0 = true; else if (v == 1) h1 = true;
        }
        if (c < 3) {
            unsigned v = val[row][c + 1];
            if (v == 0) h0 = true; else if (v == 1) h1 = true;
        }
        if (!(h0 && h1))
            return false;
    }
    return true;
}

/* Build all compatibility tables once. */
static void build_tables(void) {
    init_val();

    /* First row compatibility with a following row */
    for (int p = 0; p < 81; ++p) {
        int cnt = 0;
        for (int c = 0; c < 81; ++c) {
            if (row0_ok(p, c)) {
                first_arr[p][cnt++] = (unsigned short)c;
            }
        }
        first_cnt[p] = (unsigned char)cnt;
    }

    /* General compatibility: given a and b, find all c such that row b
       is valid when surrounded by a (up) and c (down). */
    for (int a = 0; a < 81; ++a) {
        for (int b = 0; b < 81; ++b) {
            unsigned id = a * 81 + b;
            int cnt = 0;
            for (int c = 0; c < 81; ++c) {
                bool ok = true;
                for (int col = 0; col < 4 && ok; ++col) {
                    if (!cell_ok(b, a, c, col))
                        ok = false;
                }
                if (ok)
                    compat_arr[id][cnt++] = (unsigned short)c;
            }
            compat_cnt[id] = (unsigned char)cnt;
        }
    }

    /* Last row compatibility */
    for (int p = 0; p < 81; ++p) {
        for (int c = 0; c < 81; ++c) {
            last_ok[p][c] = last_row_ok(p, c);
        }
    }

    /* Single row validity */
    for (int p = 0; p < 81; ++p)
        single_ok[p] = single_row_check(p);

    tables_ready = true;
}

/* DP arrays: dp[prev_prev][prev] holds number of ways up to current row pair */
static seq_t dp[82][82];
static seq_t nextdp[82][82];

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (!tables_ready) build_tables();

    /* initialise dp: before any rows, prev_prev = 81 sentinel, prev any */
    for (int i = 0; i < 82; ++i)
        for (int j = 0; j < 82; ++j)
            dp[i][j] = (seq_t)0;
    for (int prev = 0; prev < 81; ++prev)
        dp[81][prev] = (seq_t)1;

    /* process rows 1 .. n-1 (adding n-1 new rows) */
    for (idx_t step = 0; step + 1 < n; ++step) {
        /* zero nextdp */
        for (int i = 0; i < 82; ++i)
            for (int j = 0; j < 82; ++j)
                nextdp[i][j] = (seq_t)0;

        if (step == 0) { /* first transition: prev_prev = 81 (none) */
            for (int prev = 0; prev < 81; ++prev) {
                seq_t ways = dp[81][prev];
                if (!ways) continue;
                int cnt = first_cnt[prev];
                for (int idx = 0; idx < cnt; ++idx) {
                    int cur = first_arr[prev][idx];
                    nextdp[prev][cur] += ways;
                }
            }
        } else {
            for (int prev_prev = 0; prev_prev < 81; ++prev_prev) {
                for (int prev = 0; prev < 81; ++prev) {
                    seq_t ways = dp[prev_prev][prev];
                    if (!ways) continue;
                    unsigned id = prev_prev * 81 + prev;
                    int cnt = compat_cnt[id];
                    for (int idx = 0; idx < cnt; ++idx) {
                        int cur = compat_arr[id][idx];
                        nextdp[prev][cur] += ways;
                    }
                }
            }
        }

        /* swap dp and nextdp */
        for (int i = 0; i < 82; ++i)
            for (int j = 0; j < 82; ++j)
                dp[i][j] = nextdp[i][j];
    }

    /* compute final answer */
    seq_t ans = 0;
    if (n == 1) {
        for (int p = 0; p < 81; ++p) {
            if (single_ok[p])
                ans += dp[81][p];
        }
    } else {
        for (int prev = 0; prev < 81; ++prev) {
            for (int cur = 0; cur < 81; ++cur) {
                if (last_ok[prev][cur])
                    ans += dp[prev][cur];
            }
        }
    }
    return ans;
}
