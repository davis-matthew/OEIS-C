/*
 * A230589  Number of nX3 0..5 arrays x(i,j) with each element horizontally or
 *          vertically next to at least one element with value (x(i,j)+1) mod
 *          6, and upper left element zero.
 * https://oeis.org/A230589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A230589
 *  a(n) = number of n×3 arrays of 0..5 with upper left 0 and each cell having
 *  a horizontal or vertical neighbour equal to (x+1) mod 6.
 *  Computed by dynamic programming over 3‑column row patterns.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#define N_PAT   216          /* 6³ distinct 3‑cell patterns   */
#define N_COL   3
#define N_BITS  8            /* 2³ possible masks (0..7)      */
#define STATE_COUNT (N_PAT * N_BITS)   /* 1728 */

static uint8_t pattern_col[N_PAT][N_COL];   /* pattern lookup          */
static size_t trans_start[STATE_COUNT + 1];
static uint16_t *trans_all = NULL;
static bool init_done = false;

/* Build pattern table and all transitions.  Called once. */
static void init_precomputation(void)
{
    /* Build the pattern table. */
    for (int a = 0; a < 6; ++a)
        for (int b = 0; b < 6; ++b)
            for (int c = 0; c < 6; ++c) {
                int idx = a * 36 + b * 6 + c;
                pattern_col[idx][0] = (uint8_t)a;
                pattern_col[idx][1] = (uint8_t)b;
                pattern_col[idx][2] = (uint8_t)c;
            }

    /* First count transitions for each state. */
    int trans_count[STATE_COUNT] = {0};

    for (int prev_pat = 0; prev_pat < N_PAT; ++prev_pat) {
        for (int prev_mask = 0; prev_mask < N_BITS; ++prev_mask) {
            int s_prev = prev_pat * N_BITS + prev_mask;
            for (int cur_pat = 0; cur_pat < N_PAT; ++cur_pat) {
                /* 1. check that the row above is satisfied where required */
                bool ok = true;
                for (int j = 0; j < N_COL; ++j)
                    if ((prev_mask >> j) & 1) {
                        uint8_t req = (uint8_t)((pattern_col[prev_pat][j] + 1) % 6);
                        if (pattern_col[cur_pat][j] != req) { ok = false; break; }
                    }
                if (!ok) continue;

                /* 2. compute new mask for the current row */
                int new_mask = 0;
                for (int j = 0; j < N_COL; ++j) {
                    bool sat = false;
                    uint8_t valCur = pattern_col[cur_pat][j];

                    /* up neighbour */
                    uint8_t up = pattern_col[prev_pat][j];
                    if (up == (uint8_t)((valCur + 1) % 6)) sat = true;

                    /* left neighbour */
                    if (!sat && j > 0 &&
                        pattern_col[cur_pat][j-1] == (uint8_t)((valCur + 1) % 6))
                        sat = true;

                    /* right neighbour */
                    if (!sat && j < N_COL-1 &&
                        pattern_col[cur_pat][j+1] == (uint8_t)((valCur + 1) % 6))
                        sat = true;

                    if (!sat) new_mask |= (1 << j);
                }

                ++trans_count[s_prev];
            }
        }
    }

    /* Allocate and fill transition arrays. */
    size_t total = 0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        trans_start[s] = total;
        total += trans_count[s];
    }
    trans_start[STATE_COUNT] = total;
    trans_all = (uint16_t *)malloc(total * sizeof(uint16_t));
    if (!trans_all) exit(1);  /* allocation failed */

    /* Re‑use trans_count as a counter of inserted transitions. */
    for (int s = 0; s < STATE_COUNT; ++s) trans_count[s] = 0;

    for (int prev_pat = 0; prev_pat < N_PAT; ++prev_pat) {
        for (int prev_mask = 0; prev_mask < N_BITS; ++prev_mask) {
            int s_prev = prev_pat * N_BITS + prev_mask;
            for (int cur_pat = 0; cur_pat < N_PAT; ++cur_pat) {
                bool ok = true;
                for (int j = 0; j < N_COL; ++j)
                    if ((prev_mask >> j) & 1) {
                        uint8_t req = (uint8_t)((pattern_col[prev_pat][j] + 1) % 6);
                        if (pattern_col[cur_pat][j] != req) { ok = false; break; }
                    }
                if (!ok) continue;

                int new_mask = 0;
                for (int j = 0; j < N_COL; ++j) {
                    bool sat = false;
                    uint8_t valCur = pattern_col[cur_pat][j];

                    uint8_t up = pattern_col[prev_pat][j];
                    if (up == (uint8_t)((valCur + 1) % 6)) sat = true;

                    if (!sat && j > 0 &&
                        pattern_col[cur_pat][j-1] == (uint8_t)((valCur + 1) % 6))
                        sat = true;

                    if (!sat && j < N_COL-1 &&
                        pattern_col[cur_pat][j+1] == (uint8_t)((valCur + 1) % 6))
                        sat = true;

                    if (!sat) new_mask |= (1 << j);
                }

                int s_new = cur_pat * N_BITS + new_mask;
                trans_all[trans_start[s_prev] + trans_count[s_prev]++] = (uint16_t)s_new;
            }
        }
    }

    init_done = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!init_done) init_precomputation();

    static seq_t dp[STATE_COUNT];
    static seq_t next_dp[STATE_COUNT];

    /* Reset dp arrays */
    for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = 0;

    /* Initialize with top row. */
    for (int pat = 0; pat < N_PAT; ++pat) {
        if (pattern_col[pat][0] != 0) continue;          /* upper left must be 0 */

        int mask = 0;
        for (int j = 0; j < N_COL; ++j) {
            bool sat = false;
            uint8_t val = pattern_col[pat][j];

            /* left neighbour */
            if (j > 0 && pattern_col[pat][j-1] == (uint8_t)((val + 1) % 6))
                sat = true;
            /* right neighbour */
            if (!sat && j < N_COL-1 &&
                pattern_col[pat][j+1] == (uint8_t)((val + 1) % 6))
                sat = true;

            if (!sat) mask |= (1 << j);
        }
        dp[pat * N_BITS + mask] += (seq_t)1;
    }

    /* Process subsequent rows. */
    for (idx_t row = 2; row <= n; ++row) {
        /* clear next layer */
        for (size_t i = 0; i < STATE_COUNT; ++i) next_dp[i] = 0;

        /* apply transitions */
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = dp[s];
            if (cnt == 0) continue;
            for (size_t k = trans_start[s]; k < trans_start[s+1]; ++k) {
                int s_new = trans_all[k];
                next_dp[s_new] += cnt;
            }
        }
        /* swap dp layers */
        for (size_t i = 0; i < STATE_COUNT; ++i) dp[i] = next_dp[i];
    }

    /* Sum all states with mask == 0 (all constraints satisfied). */
    seq_t result = 0;
    for (int pat = 0; pat < N_PAT; ++pat)
        result += dp[pat * N_BITS];

    return result;
}
