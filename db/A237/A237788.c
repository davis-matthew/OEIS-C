/*
 * A237788  Number of (n+1)X(3+1) 0..3 arrays with the maximum plus the
 *          minimum minus the lower median of every 2X2 subblock differing
 *          from its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 25,
 *            big any n
 * Helpers:   t_from_block
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A237788: number of (n+1)×(3+1) 0..3 arrays satisfying the
   “t‑value” adjacency rule.  Computed by a transfer‑matrix DP over rows.
   a(n) = number of arrays of size (n+1)×4 meeting the constraints.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/t_from_block.h"

#ifndef seq_t
/* seq_t and idx_t are defined by the build system. */
#endif

/* ----- constants for the   4^4 = 256 possible rows   ----- */
#define MAX_VAL       4
#define NUM_ROWS      256            /* 4^4 */
#define NUM_T         10             /* number of admissible t‑vectors (|Δ|=1) */
#define STATE_COUNT   (NUM_ROWS * NUM_T)

/* ----- arrays describing rows and admissible t‑vectors ----- */
static uint8_t row_vals[NUM_ROWS][4];           /* row contents (0..3) */
static int8_t  tmap[4][4][4];                   /* map (t0,t1,t2) → id, -1 if invalid */
static uint8_t tvals[NUM_T][3];                 /* reverse map id → (t0,t1,t2) */

/* ----- adjacency tables used in the DP ----- */
static uint8_t next_row[NUM_ROWS][NUM_ROWS];    /* for each row i, list of admissible next rows */
static uint8_t next_tindex[NUM_ROWS][NUM_ROWS];/* the t‑vector id for (i,j) */
static uint8_t next_cnt[NUM_ROWS];              /* number of admissible next rows for each row */
static uint8_t vertical_ok[NUM_T][NUM_T];       /* vertical adjacency of t‑vectors (|Δ|=1) */

/* ----- DP storage ----- */
static seq_t dp_prev[STATE_COUNT];
static seq_t dp_next[STATE_COUNT];

/* ----- helper ----------------------------------------------------- */

/* ----- precomputation -------------------------------------------- */
static bool precomp_done = false;
static void precompute(void) {
    if (precomp_done) return;
    precomp_done = true;

    /* 1. generate all row patterns */
    for (int i = 0; i < NUM_ROWS; ++i) {
        for (int j = 0; j < 4; ++j)
            row_vals[i][j] = (uint8_t)((i >> (2 * j)) & 3);
    }

    /* 2. enumerate admissible t‑vectors (|Δ|=1) */
    int m = 0;
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            for (int k = 0; k < 4; ++k) {
                if (abs(i - j) != 1 || abs(j - k) != 1) {
                    tmap[i][j][k] = -1;
                    continue;
                }
                tvals[m][0] = (uint8_t)i;
                tvals[m][1] = (uint8_t)j;
                tvals[m][2] = (uint8_t)k;
                tmap[i][j][k] = (int8_t)m;
                ++m;
            }

    /* 3. vertical adjacency of t‑vectors: |Δ|=1 for each component */
    for (int i = 0; i < NUM_T; ++i)
        for (int j = 0; j < NUM_T; ++j) {
            bool ok = true;
            for (int k = 0; k < 3; ++k)
                if (abs(tvals[i][k] - tvals[j][k]) != 1) { ok = false; break; }
            vertical_ok[i][j] = ok ? 1 : 0;
        }

    /* 4. for each row compute admissible next rows and t‑vector id */
    for (int i = 0; i < NUM_ROWS; ++i) {
        int cnt = 0;
        for (int j = 0; j < NUM_ROWS; ++j) {
            int t[3];
            for (int k = 0; k < 3; ++k) {
                int block[4];
                block[0] = row_vals[i][k];
                block[1] = row_vals[i][k + 1];
                block[2] = row_vals[j][k];
                block[3] = row_vals[j][k + 1];
                t[k] = t_from_block(block);
            }
            /* horizontal differences must be 1 */
            if (abs(t[0] - t[1]) != 1 || abs(t[1] - t[2]) != 1)
                continue;
            int tidx = tmap[t[0]][t[1]][t[2]];
            if (tidx < 0) continue;       /* safety check */
            next_row[i][cnt]    = (uint8_t)j;
            next_tindex[i][cnt] = (uint8_t)tidx;
            ++cnt;
        }
        next_cnt[i] = (uint8_t)cnt;
    }
}

/* ----- public API ------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* below offset; behaviour irrelevant */
    precompute();                 /* build tables once */

    /* clear dp_prev */
    for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = 0;

    /* initial counting for the first two rows (no vertical restriction yet) */
    for (int r0 = 0; r0 < NUM_ROWS; ++r0) {
        int cnt = next_cnt[r0];
        for (int idx = 0; idx < cnt; ++idx) {
            int tIdx = next_tindex[r0][idx];
            int state = r0 * NUM_T + tIdx;
            dp_prev[state] += (seq_t)1;
        }
    }

    /* add remaining rows */
    idx_t steps = n - 1;                  /* number of additional rows to append */
    for (idx_t s = 0; s < steps; ++s) {
        /* zero dp_next */
        for (int i = 0; i < STATE_COUNT; ++i) dp_next[i] = 0;

        /* transition */
        for (int r0 = 0; r0 < NUM_ROWS; ++r0) {
            int base = r0 * NUM_T;
            int cnt = next_cnt[r0];
            for (int ti = 0; ti < NUM_T; ++ti) {
                seq_t val = dp_prev[base + ti];
                if (!val) continue;
                for (int k = 0; k < cnt; ++k) {
                    int r1   = next_row[r0][k];
                    int tIdx = next_tindex[r0][k];
                    if (!vertical_ok[ti][tIdx]) continue;
                    int target = r1 * NUM_T + tIdx;
                    dp_next[target] += val;
                }
            }
        }

        /* swap dp_next into dp_prev for the next iteration */
        for (int i = 0; i < STATE_COUNT; ++i) dp_prev[i] = dp_next[i];
    }

    /* final sum of all states */
    seq_t ans = 0;
    for (int i = 0; i < STATE_COUNT; ++i) ans += dp_prev[i];
    return ans;
}
