/*
 * A237400  Number of (n+1) X (2+1) 0..3 arrays with the maximum plus the
 *          upper median plus the lower median minus the minimum of every 2 X
 *          2 subblock differing from its horizontal and vertical neighbors by
 *          exactly one.
 * https://oeis.org/A237400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   dynamic programming over rows for A237400 * Number of (n+1)×3
 *            arrays with entries 0..3 * where
 *            max+upper_median+lower_median−min for each 2×2 subblock * has
 *            consecutive differences of exactly one horizontally and
 *            vertically
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   subblock_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/subblock_val.h"

#define ROWS       64          /* 4^3 possible rows */
#define STATE_MULT 100         /* 10*10 for m0,m1 */
#define STATE_CNT  (ROWS * STATE_MULT)   /* 6400 states */

static uint8_t row_val[ROWS][3];
static bool    pair_valid[ROWS][ROWS];
static uint8_t pair_m0[ROWS][ROWS];
static uint8_t pair_m1[ROWS][ROWS];

static size_t *adj_offset = NULL;  /* offset in flat adjacency array */
static size_t *adj_edges  = NULL;  /* adjacency list (flat) */
static size_t  adj_edges_cnt = 0;
static bool    precomp_done = false;

static void precompute(void)
{
    /* rows */
    for (uint32_t r = 0; r < ROWS; ++r) {
        uint32_t t = r;
        for (int i = 0; i < 3; ++i) {
            row_val[r][i] = t & 3;
            t >>= 2;
        }
    }

    /* pair values */
    for (uint32_t a = 0; a < ROWS; ++a)
        for (uint32_t b = 0; b < ROWS; ++b) {
            uint8_t a0 = row_val[a][0], a1 = row_val[a][1], a2 = row_val[a][2];
            uint8_t b0 = row_val[b][0], b1 = row_val[b][1], b2 = row_val[b][2];
            uint8_t m0 = subblock_val(a0, a1, b0, b1);
            uint8_t m1 = subblock_val(a1, a2, b1, b2);
            if ((m0 > m1 ? m0 - m1 : m1 - m0) == 1) {
                pair_valid[a][b] = true;
                pair_m0[a][b]    = m0;
                pair_m1[a][b]    = m1;
            } else {
                pair_valid[a][b] = false;
            }
        }

    /* degrees counting */
    size_t deg[STATE_CNT];
    for (size_t s = 0; s < STATE_CNT; ++s) deg[s] = 0;
    for (size_t s = 0; s < STATE_CNT; ++s) {
        uint32_t row = s / STATE_MULT;
        uint32_t m0  = (s / 10) % 10;
        uint32_t m1  = s % 10;
        for (uint32_t nxt = 0; nxt < ROWS; ++nxt) {
            if (!pair_valid[row][nxt]) continue;
            uint32_t cm0 = pair_m0[row][nxt];
            uint32_t cm1 = pair_m1[row][nxt];
            if (((m0 > cm0 ? m0 - cm0 : cm0 - m0) == 1) &&
                ((m1 > cm1 ? m1 - cm1 : cm1 - m1) == 1))
                ++deg[s];
        }
    }

    /* offsets */
    adj_offset = (size_t *)malloc((STATE_CNT + 1) * sizeof(size_t));
    adj_offset[0] = 0;
    for (size_t s = 0; s < STATE_CNT; ++s)
        adj_offset[s + 1] = adj_offset[s] + deg[s];
    adj_edges_cnt = adj_offset[STATE_CNT];

    /* adjacency flat list */
    adj_edges = (size_t *)malloc(adj_edges_cnt * sizeof(size_t));
    size_t cur[STATE_CNT];
    for (size_t s = 0; s < STATE_CNT; ++s) cur[s] = adj_offset[s];

    for (size_t s = 0; s < STATE_CNT; ++s) {
        uint32_t row = s / STATE_MULT;
        uint32_t m0  = (s / 10) % 10;
        uint32_t m1  = s % 10;
        for (uint32_t nxt = 0; nxt < ROWS; ++nxt) {
            if (!pair_valid[row][nxt]) continue;
            uint32_t cm0 = pair_m0[row][nxt];
            uint32_t cm1 = pair_m1[row][nxt];
            if (((m0 > cm0 ? m0 - cm0 : cm0 - m0) == 1) &&
                ((m1 > cm1 ? m1 - cm1 : cm1 - m1) == 1)) {
                size_t dest = (size_t)nxt * STATE_MULT + cm0 * 10 + cm1;
                adj_edges[cur[s]++] = dest;
            }
        }
    }

    precomp_done = true;
}

seq_t a(idx_t n)
{
    if (!precomp_done) precompute();

    if (n == 0) return 0;   /* below offset, undefined but safe */

    size_t max_states = STATE_CNT;
    seq_t *dp   = (seq_t *)calloc(max_states, sizeof(seq_t));
    seq_t *dp2  = (seq_t *)calloc(max_states, sizeof(seq_t));
    if (!dp || !dp2) {
        /* allocation failure: return 0 */
        return 0;
    }

    /* initial dp from first pair */
    for (uint32_t r0 = 0; r0 < ROWS; ++r0)
        for (uint32_t r1 = 0; r1 < ROWS; ++r1) {
            if (!pair_valid[r0][r1]) continue;
            size_t state = (size_t)r1 * STATE_MULT
                         + pair_m0[r0][r1] * 10
                         + pair_m1[r0][r1];
            dp[state] += 1;
        }

    /* advance for remaining pairs */
    for (idx_t step = 1; step < n; ++step) {
        /* zero dp2 */
        memset(dp2, 0, max_states * sizeof(seq_t));
        for (size_t s = 0; s < max_states; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            size_t start = adj_offset[s];
            size_t end   = adj_offset[s + 1];
            for (size_t idx = start; idx < end; ++idx) {
                size_t dest = adj_edges[idx];
                dp2[dest] += val;
            }
        }
        /* swap dp and dp2 pointers */
        seq_t *tmp = dp; dp = dp2; dp2 = tmp;
    }

    /* sum all dp[s] */
    seq_t ans = 0;
    for (size_t s = 0; s < max_states; ++s)
        ans += dp[s];

    free(dp);
    free(dp2);
    return ans;
}
