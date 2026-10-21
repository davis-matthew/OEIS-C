/*
 * A238307  Number of (n+1)X(2+1) 0..3 arrays with the maximum minus the lower
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A238307
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   second_smallest_4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A238307
 * Computed by dynamic programming with a state‑transition model.
 * For each row of the 3‑column grid a state encodes the previous row
 * and the difference values of its two 2×2 subblocks.  Transition
 * rules enforce the “exactly one” neighbor condition.
 */
#include <stdint.h>
#include "../utils/second_smallest_4.h"

#define STATE_ROWS   64
#define MAX_TRANS    (STATE_ROWS)          /* at most 64 successors */
#define STATES       (STATE_ROWS * 4 * 4)   /* 1024 */

static uint8_t dl[STATE_ROWS][STATE_ROWS];
static uint8_t dr[STATE_ROWS][STATE_ROWS];
static uint8_t horiz_ok[STATE_ROWS][STATE_ROWS];

static uint16_t trans_to[STATES][MAX_TRANS];
static uint8_t trans_wt[STATES][MAX_TRANS];
static uint8_t trans_cnt[STATES];

static seq_t initial_counts[STATES];
static uint8_t init_flag = 0;

/* pre‑compute tables once */
static void init_tables(void)
{
    if (init_flag) return;
    init_flag = 1;

    /* compute d_left, d_right and horizontal feasibility for every row pair */
    for (int r1 = 0; r1 < STATE_ROWS; ++r1) {
        uint8_t a0 =  (r1 >> 4) & 3;
        uint8_t a1 =  (r1 >> 2) & 3;
        uint8_t a2 =   r1       & 3;
        for (int r2 = 0; r2 < STATE_ROWS; ++r2) {
            uint8_t b0 =  (r2 >> 4) & 3;
            uint8_t b1 =  (r2 >> 2) & 3;
            uint8_t b2 =   r2       & 3;

            /* left 2×2 block */
            uint8_t maxL = a0;
            if (a1 > maxL) maxL = a1;
            if (b0 > maxL) maxL = b0;
            if (b1 > maxL) maxL = b1;
            uint8_t lowL = second_smallest_4(a0,a1,b0,b1);
            uint8_t dL = maxL - lowL;

            /* right 2×2 block */
            uint8_t maxR = a1;
            if (a2 > maxR) maxR = a2;
            if (b1 > maxR) maxR = b1;
            if (b2 > maxR) maxR = b2;
            uint8_t lowR = second_smallest_4(a1,a2,b1,b2);
            uint8_t dR = maxR - lowR;

            if ( (dL > dR ? dL-dR : dR-dL) == 1 ) {
                horiz_ok[r1][r2] = 1;
                dl[r1][r2] = dL;
                dr[r1][r2] = dR;
            } else {
                horiz_ok[r1][r2] = 0;
            }
        }
    }

    /* build transition lists */
    for (int i = 0; i < STATES; ++i) trans_cnt[i] = 0;
    for (int prev = 0; prev < STATE_ROWS; ++prev) {
        for (int d_prev_l = 0; d_prev_l < 4; ++d_prev_l) {
            for (int d_prev_r = 0; d_prev_r < 4; ++d_prev_r) {
                int state = (prev << 4) | (d_prev_l << 2) | d_prev_r;
                for (int next = 0; next < STATE_ROWS; ++next) {
                    if (!horiz_ok[prev][next]) continue;
                    uint8_t dL = dl[prev][next];
                    uint8_t dR = dr[prev][next];
                    if ( ((dL > d_prev_l ? dL-d_prev_l : d_prev_l-dL) != 1) ||
                         ((dR > d_prev_r ? dR-d_prev_r : d_prev_r-dR) != 1) )
                        continue;
                    int new_state = (next << 4) | (dL << 2) | dR;

                    uint8_t k = trans_cnt[state];
                    /* check for existing transition */
                    int existing = -1;
                    for (int t = 0; t < k; ++t) {
                        if (trans_to[state][t] == new_state) { existing = t; break; }
                    }
                    if (existing >= 0) {
                        trans_wt[state][existing] += 1;
                    } else {
                        trans_to[state][k] = (uint16_t)new_state;
                        trans_wt[state][k] = 1;
                        trans_cnt[state] = k + 1;
                    }
                }
            }
        }
    }

    /* initial counts after two rows */
    for (int i = 0; i < STATES; ++i) initial_counts[i] = 0;
    for (int r0 = 0; r0 < STATE_ROWS; ++r0) {
        for (int r1 = 0; r1 < STATE_ROWS; ++r1) {
            if (!horiz_ok[r0][r1]) continue;
            uint8_t dL = dl[r0][r1];
            uint8_t dR = dr[r0][r1];
            int state = (r0 << 4) | (dL << 2) | dR;
            initial_counts[state] += (seq_t)1;
        }
    }
}

seq_t a(idx_t n)
{
    init_tables();
    if (n == 0) return 0;        /* undefined, but safe */

    seq_t cur[STATES];
    seq_t nxt[STATES];

    for (int i = 0; i < STATES; ++i) cur[i] = initial_counts[i];

    if (n > 1) {
        for (int step = 0; step < (int)(n - 1); ++step) {
            for (int i = 0; i < STATES; ++i) nxt[i] = 0;
            for (int s = 0; s < STATES; ++s) {
                seq_t val = cur[s];
                if (val == 0) continue;
                uint8_t cnt = trans_cnt[s];
                for (uint8_t t = 0; t < cnt; ++t) {
                    int j = trans_to[s][t];
                    unsigned int weight = trans_wt[s][t];
                    seq_t prod = val * (seq_t)weight;
                    nxt[j] += prod;
                }
            }
            for (int i = 0; i < STATES; ++i) cur[i] = nxt[i];
        }
    }

    seq_t total = 0;
    for (int s = 0; s < STATES; ++s) total += cur[s];
    return total;
}
