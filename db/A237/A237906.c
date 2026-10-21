/*
 * A237906  Number of (n+1)X(2+1) 0..3 arrays with the maximum minus the upper
 *          median of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A237906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   DP over all 64 possible 3‑cell rows and the 6 possible
 *            differences (max minus upper median) of 2x2 subblocks. The DP
 *            counts (n+1)×3 arrays whose block differences differ
 *            horizontally and vertically by exactly one. The implementation
 *            uses an automaton with 384 states (64 rows × 6 block‑diff pairs)
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 33,
 *            big any n
 * Helpers:   abs_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/abs_int.h"

/* Map from difference pairs to indices 0..5 */
static int  val_idx_map[4][4];

/* The actual difference values for each index */
static int  val1_val[6];
static int  val2_val[6];

/* Information about a pair of consecutive rows */
typedef struct {
    bool  ok;    /* whether the pair satisfies the vertical constraint */
    int   v1;    /* block difference for the first column pair */
    int   v2;    /* block difference for the second column pair */
    int   v_idx; /* index (0..5) of the pair if ok */
} pair_info_t;

static pair_info_t pair_info[64][64];

/* Transition table: for each state (row, v_idx) the list of next
   states.  We store state indices in a 16‑bit value because the
   maximum state is 383. */
static uint16_t trans_next[384][64];
static unsigned char trans_cnt[384];   /* number of transitions per state */

/* Absolute value of an int */

/* Pre‑compute all tables needed for the DP */
static void precompute(void)
{
    int i, j, k, idx, ridx;
    /* Initialise all entries of the value‑index map to -1 */
    for (i = 0; i < 4; ++i)
        for (j = 0; j < 4; ++j)
            val_idx_map[i][j] = -1;

    /* The six admissible difference pairs (max‑median differences)
       for a 2×2 block are: (0,1), (1,0), (1,2), (2,1), (2,3), (3,2). */
    val_idx_map[0][1] = 0;  val1_val[0] = 0; val2_val[0] = 1;
    val_idx_map[1][0] = 1;  val1_val[1] = 1; val2_val[1] = 0;
    val_idx_map[1][2] = 2;  val1_val[2] = 1; val2_val[2] = 2;
    val_idx_map[2][1] = 3;  val1_val[3] = 2; val2_val[3] = 1;
    val_idx_map[2][3] = 4;  val1_val[4] = 2; val2_val[4] = 3;
    val_idx_map[3][2] = 5;  val1_val[5] = 3; val2_val[5] = 2;

    /* Compute pair_info for every pair of rows (64×64). */
    for (idx = 0; idx < 64; ++idx) {
        int a0 = idx & 3;
        int a1 = (idx >> 2) & 3;
        int a2 = (idx >> 4) & 3;

        for (ridx = 0; ridx < 64; ++ridx) {
            int b0 = ridx & 3;
            int b1 = (ridx >> 2) & 3;
            int b2 = (ridx >> 4) & 3;

            /* First 2×2 block: columns 1–2 */
            int v[4] = {a0, a1, b0, b1};
            int mx1 = -1, mx2 = -1;
            for (k = 0; k < 4; ++k) {
                int vcur = v[k];
                if (vcur > mx1) { mx2 = mx1; mx1 = vcur; }
                else if (vcur > mx2) mx2 = vcur;
            }
            int v1 = mx1 - mx2;

            /* Second 2×2 block: columns 2–3 */
            v[0] = a1; v[1] = a2; v[2] = b1; v[3] = b2;
            mx1 = -1; mx2 = -1;
            for (k = 0; k < 4; ++k) {
                int vcur = v[k];
                if (vcur > mx1) { mx2 = mx1; mx1 = vcur; }
                else if (vcur > mx2) mx2 = vcur;
            }
            int v2 = mx1 - mx2;

            if (abs_int(v1 - v2) == 1) {
                int vidx = val_idx_map[v1][v2];
                pair_info[idx][ridx].ok    = true;
                pair_info[idx][ridx].v1    = v1;
                pair_info[idx][ridx].v2    = v2;
                pair_info[idx][ridx].v_idx = vidx;
            } else {
                pair_info[idx][ridx].ok = false;
            }
        }
    }

    /* Build transition table for the automaton */
    for (int state = 0; state < 384; ++state) trans_cnt[state] = 0;

    for (int row = 0; row < 64; ++row) {
        for (int vidx = 0; vidx < 6; ++vidx) {
            int state = row * 6 + vidx;
            for (int nrow = 0; nrow < 64; ++nrow) {
                pair_info_t pi = pair_info[row][nrow];
                if (!pi.ok) continue;
                int vnew = pi.v_idx;
                if (abs_int(val1_val[vidx] - val1_val[vnew]) == 1 &&
                    abs_int(val2_val[vidx] - val2_val[vnew]) == 1) {
                    int next_state = nrow * 6 + vnew;
                    trans_next[state][trans_cnt[state]] = (uint16_t)next_state;
                    trans_cnt[state] += 1;
                }
            }
        }
    }
}

/* Main entry point for the OEIS sequence */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        precompute();
        initialized = true;
    }

    if (n == 0) return (seq_t)0;   /* undefined for n < 1, behaviour arbitrary */

    /* DP arrays */
    seq_t dp_cur[384];
    seq_t dp_next[384];
    unsigned i, j;

    /* Initialise dp_cur with all admissible first pair of rows */
    for (i = 0; i < 384; ++i) dp_cur[i] = 0;
    for (i = 0; i < 64; ++i) {
        for (j = 0; j < 64; ++j) {
            pair_info_t pi = pair_info[i][j];
            if (pi.ok) {
                unsigned state = j * 6 + pi.v_idx;
                dp_cur[state] += 1;
            }
        }
    }

    /* For n == 1 (two rows) we are already done. */
    if (n == 1) {
        seq_t total = 0;
        for (i = 0; i < 384; ++i) total += dp_cur[i];
        return total;
    }

    /* Add rows one by one */
    for (idx_t step = 2; step <= n; ++step) {
        for (i = 0; i < 384; ++i) dp_next[i] = 0;
        for (i = 0; i < 384; ++i) {
            seq_t val = dp_cur[i];
            if (val == 0) continue;
            unsigned cnt = trans_cnt[i];
            for (j = 0; j < cnt; ++j) {
                unsigned nxt = trans_next[i][j];
                dp_next[nxt] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (i = 0; i < 384; ++i) dp_cur[i] = dp_next[i];
    }

    /* Sum all final states */
    seq_t result = 0;
    for (i = 0; i < 384; ++i) result += dp_cur[i];
    return result;
}
