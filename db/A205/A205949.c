/*
 * A205949  Number of (n+1) X 3 0..3 arrays with no 2 X 2 subblock having the
 *          same number of clockwise edge increases as its horizontal
 *          neighbors or the same number of counterclockwise edge increases as
 *          its vertical neighbors.
 * https://oeis.org/A205949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming on 3‑column rows with
   a transfer matrix that enforces the clockwise and counter‑clockwise
   edge increase constraints. */

#include <stdint.h>

#define ROWS 64           /* 4^3 possible rows */
#define COUNTS 5          /* counter values 0..4 */
#define STATE_W 25        /* ROWS * COUNTS */
#define STATES (ROWS * STATE_W)   /* 1600 states */
#define MAX_TRANS 64      /* maximum outgoing transitions per state */

/* encode a state: row (0..63), counters c0 and c1 (0..4) */
static inline int state_id(int row, int c0, int c1) {
    return row * STATE_W + c0 * 5 + c1;
}

/* digits of a row index (base‑4 representation) */
static unsigned char row_digit[ROWS][3];

/* precomputed counts for a pair of rows:
   clk[2]  – clockwise edge increases for subblocks j=0,1
   cnt[2]  – counter‑clockwise edge increases for subblocks j=0,1 */
typedef struct {
    unsigned char clk[2];
    unsigned char cnt[2];
} PairInfo;
static PairInfo pair_info[ROWS][ROWS];

/* adjacency list for transitions */
static unsigned short trans_target[STATES][MAX_TRANS];
static unsigned char  trans_cnt[STATES];

/* initialise tables once (static vectors are zeroed) */
static void init_tables(void) {
    /* row digits */
    for (int r = 0; r < ROWS; ++r) {
        int tmp = r;
        for (int d = 0; d < 3; ++d) {
            row_digit[r][d] = tmp & 3;
            tmp >>= 2;
        }
    }

    /* pair information */
    for (int r1 = 0; r1 < ROWS; ++r1) {
        for (int r2 = 0; r2 < ROWS; ++r2) {
            PairInfo *p = &pair_info[r1][r2];
            for (int j = 0; j < 2; ++j) {
                int TL = row_digit[r1][j];
                int TR = row_digit[r1][j + 1];
                int BL = row_digit[r2][j];
                int BR = row_digit[r2][j + 1];
                int clk = (TL < TR) + (TR < BR) + (BR < BL) + (BL < TL);
                int cnt = (TL < BL) + (BL < BR) + (BR < TR) + (TR < TL);
                p->clk[j] = (unsigned char)clk;
                p->cnt[j] = (unsigned char)cnt;
            }
        }
    }

    /* transitions */
    for (int s = 0; s < STATES; ++s) {
        int row = s / STATE_W;
        int c0 = (s / 5) % 5;
        int c1 = s % 5;
        unsigned char cnt = 0;
        for (int r2 = 0; r2 < ROWS; ++r2) {
            PairInfo *p = &pair_info[row][r2];
            if (p->cnt[0] == c0 || p->cnt[1] == c1) continue;
            if (p->clk[0] == p->clk[1]) continue;
            int t = state_id(r2, p->cnt[0], p->cnt[1]);
            trans_target[s][cnt++] = (unsigned short)t;
        }
        trans_cnt[s] = cnt;
    }
}

/* flush tables once at first use */
static int init_done = 0;

seq_t a(idx_t n) {
    if (!init_done) {
        init_tables();
        init_done = 1;
    }

    /* dp arrays */
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* initialise dp_cur with first pair (rows 0 and 1) */
    for (int i = 0; i < STATES; ++i) {
        dp_cur[i] = 0;
    }
    for (int r0 = 0; r0 < ROWS; ++r0) {
        for (int r1 = 0; r1 < ROWS; ++r1) {
            PairInfo *p = &pair_info[r0][r1];
            if (p->clk[0] != p->clk[1]) {
                int id = state_id(r1, p->cnt[0], p->cnt[1]);
                dp_cur[id] += 1;
            }
        }
    }

    /* number of additional transitions needed */
    idx_t steps = (n > 1) ? n - 1 : 0;
    for (idx_t step = 0; step < steps; ++step) {
        /* zero dp_next */
        for (int i = 0; i < STATES; ++i) {
            dp_next[i] = 0;
        }
        /* propagate transitions */
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            unsigned char tcnt = trans_cnt[s];
            for (unsigned char k = 0; k < tcnt; ++k) {
                int t = trans_target[s][k];
                dp_next[t] += val;
            }
        }
        /* copy dp_next -> dp_cur for next iteration */
        for (int i = 0; i < STATES; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }

    /* sum all final states */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) {
        result += dp_cur[i];
    }
    return result;
}
