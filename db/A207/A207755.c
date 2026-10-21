/*
 * A207755  Number of 6Xn 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 0 vertically.
 * https://oeis.org/A207755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 6×n binary arrays avoiding * the vertical
 *            patterns 001 and 100, and the horizontal patterns * 0 0 1 and 0
 *            1 1. * This is computed by a DP over column pairs. * * a(1) = 22
 *            * a(2) = 22² = 484 * a(n) for n≥3 is obtained by expanding one
 *            column at a time * while respecting all forbid...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 29, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

/* Helper – convert idx_t to seq_t before arithmetic */
static inline seq_t to_seq(idx_t x) { return (seq_t)x; }

/* Global pre‑computation data (initialised once) */
static bool init_done = false;

static bool col_valid[64];
static uint8_t valid_cols[64];
static size_t valid_cnt = 0;

/* adjacency list for each ordered pair (c1,c2)    */
static size_t *adj_start = NULL;  /* start index into adj_buf   */
static uint8_t *adj_len   = NULL;  /* number of admissible c3   */
static uint8_t *adj_buf   = NULL;  /* concatenated c3 values   */

/* Perform one‑time precomputation */
static void precompute(void)
{
    /* 1. Enumerate vertical‑valid columns */
    for (uint8_t col = 0; col < 64; ++col) {
        bool ok = true;
        for (int i = 0; i <= 3; ++i) {
            uint8_t b0 = (col >> i)     & 1;
            uint8_t b1 = (col >> (i+1)) & 1;
            uint8_t b2 = (col >> (i+2)) & 1;
            if ((b0 == 0 && b1 == 0 && b2 == 1) ||
                (b0 == 1 && b1 == 0 && b2 == 0)) {
                ok = false;
                break;
            }
        }
        col_valid[col] = ok;
        if (ok) {
            valid_cols[valid_cnt++] = col;
        }
    }

    /* 2. Build adjacency lists */
    size_t total = 0;
    size_t cnt_pair[4096] = {0};

    for (int c1 = 0; c1 < 64; ++c1) {
        if (!col_valid[c1]) continue;
        for (int c2 = 0; c2 < 64; ++c2) {
            if (!col_valid[c2]) continue;

            size_t idx = ((size_t)c1 << 6) | c2;
            size_t len = 0;
            for (size_t vi = 0; vi < valid_cnt; ++vi) {
                uint8_t c3 = valid_cols[vi];
                bool ok = true;
                for (int i = 0; i < 6 && ok; ++i) {
                    uint8_t b1 = (c1 >> i) & 1;
                    uint8_t b2 = (c2 >> i) & 1;
                    uint8_t b3 = (c3 >> i) & 1;
                    if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                        (b1 == 0 && b2 == 1 && b3 == 1)) {
                        ok = false;
                    }
                }
                if (ok) ++len;
            }
            cnt_pair[idx] = len;
            total += len;
        }
    }

    adj_start = (size_t*)malloc(4096 * sizeof(size_t));
    adj_len   = (uint8_t*)malloc(4096 * sizeof(uint8_t));
    adj_buf   = (uint8_t*)malloc(total   * sizeof(uint8_t));

    /* 3. Populate adjacency buffers */
    size_t pos = 0;
    for (int c1 = 0; c1 < 64; ++c1) {
        if (!col_valid[c1]) continue;
        for (int c2 = 0; c2 < 64; ++c2) {
            if (!col_valid[c2]) continue;
            size_t idx = ((size_t)c1 << 6) | c2;
            uint8_t len = (uint8_t)cnt_pair[idx];
            adj_len[idx] = len;
            adj_start[idx] = pos;
            for (size_t vi = 0; vi < valid_cnt; ++vi) {
                uint8_t c3 = valid_cols[vi];
                bool ok = true;
                for (int i = 0; i < 6 && ok; ++i) {
                    uint8_t b1 = (c1 >> i) & 1;
                    uint8_t b2 = (c2 >> i) & 1;
                    uint8_t b3 = (c3 >> i) & 1;
                    if ((b1 == 0 && b2 == 0 && b3 == 1) ||
                        (b1 == 0 && b2 == 1 && b3 == 1)) {
                        ok = false;
                    }
                }
                if (ok) adj_buf[pos++] = c3;
            }
        }
    }

    init_done = true;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (!init_done) precompute();

    if (n == 0) return (seq_t)0;          /* undefined offset */
    if (n == 1) return to_seq(valid_cnt); /* single column */

    /* For n==2 the board has two columns, no horizontal triples exist */
    if (n == 2) return to_seq(valid_cnt) * to_seq(valid_cnt);

    /* DP over column pairs for length >=3 */
    static seq_t dp[2][64][64];
    int cur = 0, next = 1;

    /* Initialise after 2 columns: every pair of vertical‑valid columns is allowed */
    memset(dp[cur], 0, sizeof(dp[cur]));
    for (int c1 = 0; c1 < 64; ++c1) {
        if (!col_valid[c1]) continue;
        for (int c2 = 0; c2 < 64; ++c2) {
            if (!col_valid[c2]) continue;
            dp[cur][c1][c2] = (seq_t)1;
        }
    }

    /* Build columns 3 .. n */
    for (idx_t step = 3; step <= n; ++step) {
        memset(dp[next], 0, sizeof(dp[next]));
        for (int c1 = 0; c1 < 64; ++c1) {
            if (!col_valid[c1]) continue;
            for (int c2 = 0; c2 < 64; ++c2) {
                if (!col_valid[c2]) continue;
                seq_t val = dp[cur][c1][c2];
                if (val == 0) continue;
                size_t idx = ((size_t)c1 << 6) | c2;
                uint8_t len = adj_len[idx];
                size_t pos = adj_start[idx];
                for (uint8_t k = 0; k < len; ++k) {
                    uint8_t c3 = adj_buf[pos + k];
                    dp[next][c2][c3] += val;
                }
            }
        }
        /* swap */
        int t = cur; cur = next; next = t;
    }

    /* Sum all terminal states */
    seq_t answer = 0;
    for (int c1 = 0; c1 < 64; ++c1) {
        for (int c2 = 0; c2 < 64; ++c2) {
            answer += dp[cur][c1][c2];
        }
    }
    return answer;
}
