/*
 * A205200  Number of (n+1)X3 0..2 arrays with the permanents of 2X2 subblocks
 *          nondecreasing rightwards and downwards.
 * https://oeis.org/A205200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..2 whose 2×2
 *            block permanents are nondecreasing rightwards and downwards
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int initialized = 0;

#define ROWS           27                /* 3^3 rows of 0..2 values */
#define ALT_BET_MAX    9                 /* permanents range 0..8 */
#define STATE_COUNT    (ROWS*ALT_BET_MAX*ALT_BET_MAX)   /* 2187 */
#define MAX_TRANS      27                /* at most one next row per transition */

/* Pre‑computed data */
static uint8_t rows[ROWS][3];
static uint8_t perm1[ROWS][ROWS];
static uint8_t perm2[ROWS][ROWS];
static uint8_t out_cnt[STATE_COUNT];
static uint16_t out_next[STATE_COUNT][MAX_TRANS];

/* Dynamic programming tables */
static seq_t dp_cur[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

seq_t a(idx_t n)
{
    /* n = 0 is not defined in the sequence, but we return 0 for safety */
    if (n == 0) return 0;

    /* first‑time initialisation of static tables */
    if (!initialized) {
        /* generate all possible rows of length 3 over {0,1,2} */
        for (int a = 0; a < 3; ++a)
            for (int b = 0; b < 3; ++b)
                for (int c = 0; c < 3; ++c) {
                    int idx = a * 9 + b * 3 + c;
                    rows[idx][0] = (uint8_t)a;
                    rows[idx][1] = (uint8_t)b;
                    rows[idx][2] = (uint8_t)c;
                }

        /* compute permanents of all 2×2 subblocks between two rows */
        for (int i = 0; i < ROWS; ++i) {
            uint8_t *ri = rows[i];
            for (int j = 0; j < ROWS; ++j) {
                uint8_t *rj = rows[j];
                perm1[i][j] = ri[0] * rj[1] + ri[1] * rj[0]; /* columns 1-2 */
                perm2[i][j] = ri[1] * rj[2] + ri[2] * rj[1]; /* columns 2-3 */
            }
        }

        /* build transition lists for all states */
        for (int state = 0; state < STATE_COUNT; ++state) {
            int row_prev = state / (ALT_BET_MAX * ALT_BET_MAX);
            int prev_alt = (state / ALT_BET_MAX) % ALT_BET_MAX;
            int prev_bet = state % ALT_BET_MAX;

            uint8_t cnt = 0;
            for (int row_next = 0; row_next < ROWS; ++row_next) {
                int new_alt = perm1[row_prev][row_next];
                int new_bet = perm2[row_prev][row_next];
                /* vertical nondecreasing (prev <= new) and horizontal (new_alt <= new_bet) */
                if (prev_alt <= new_alt && prev_bet <= new_bet && new_alt <= new_bet) {
                    int next_state = row_next * (ALT_BET_MAX * ALT_BET_MAX)
                                     + new_alt * ALT_BET_MAX + new_bet;
                    out_next[state][cnt++] = (uint16_t)next_state;
                }
            }
            out_cnt[state] = cnt;
        }

        initialized = 1;
    }

    /* zero dynamic tables for this call */
    for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = 0;

    /* base case: first pair of rows (n = 1) */
    for (int r0 = 0; r0 < ROWS; ++r0)
        for (int r1 = 0; r1 < ROWS; ++r1) {
            int alt = perm1[r0][r1];
            int bet = perm2[r0][r1];
            if (alt <= bet) {
                int state = r1 * (ALT_BET_MAX * ALT_BET_MAX)
                            + alt * ALT_BET_MAX + bet;
                dp_cur[state] += (seq_t)1;
            }
        }

    /* if only one pair of rows, the answer is the sum of base states */
    if (n == 1) {
        seq_t ans = 0;
        for (int i = 0; i < STATE_COUNT; ++i) ans += dp_cur[i];
        return ans;
    }

    /* DP for subsequent rows (steps 2 … n) */
    for (idx_t step = 2; step <= n; ++step) {
        /* clear next array */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* propagate transitions */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            uint8_t cnt = out_cnt[s];
            for (uint8_t k = 0; k < cnt; ++k) {
                int t = out_next[s][k];
                dp_next[t] += val;
            }
        }

        /* copy next into current for next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all final states to get the answer */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp_cur[i];
    return ans;
}
