/*
 * A238066  Number of (n+1)X(2+1) 0..3 arrays with the maximum minus the
 *          minimum of every 2X2 subblock differing from its horizontal and
 *          vertical neighbors by exactly one.
 * https://oeis.org/A238066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   absdiff_uint8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A238066: Number of (n+1)×3 arrays 0..3 with the 2×2 subblock d‑values
   (max–min) differing from their horizontal and vertical neighbours by 1.
   Computed by dynamic programming over adjacent row pairs. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/absdiff_uint8.h"

#define MAX_ROWS      64                  /* 3 cells in a row, coded in base‑4 */
#define MAX_STATES    (MAX_ROWS * MAX_ROWS)

/* Pre‑computed tables */
static uint8_t  row_cells[MAX_ROWS][3];        /* decoded values of a row */
static uint8_t  pair_d0[MAX_ROWS][MAX_ROWS];  /* d for columns 0‑1 of the pair */
static uint8_t  pair_d1[MAX_ROWS][MAX_ROWS];  /* d for columns 1‑2 of the pair */
static bool     valid_pair[MAX_ROWS][MAX_ROWS];/* true if horizontal constraint holds */

static int  state_index[MAX_ROWS][MAX_ROWS];  /* -1 if the pair is not a state */
static int  state_top[MAX_STATES];            /* top row of the state */
static int  state_bot[MAX_STATES];            /* bottom row of the state */
static int  state_d0 [MAX_STATES];            /* d0 of the state */
static int  state_d1 [MAX_STATES];            /* d1 of the state */
static int  state_count;                     /* number of valid states */

static int  out_start[MAX_STATES + 1];        /* prefix sums of outgoing edges */
static int  out_count[MAX_STATES];            /* number of outgoing edges per state */
static int *out_edges;                       /* concatenated list of target state indices */

static seq_t dp0[MAX_STATES];
static seq_t dp1[MAX_STATES];
static bool initialized = false;

/* One‑time pre‑computation */
static void init_precomputed(void)
{
    if (initialized) return;
    initialized = true;

    /* Decode all 64 possible rows */
    for (int r = 0; r < MAX_ROWS; ++r) {
        int v = r;
        for (int c = 0; c < 3; ++c) {
            row_cells[r][c] = (uint8_t)(v & 3);
            v >>= 2;
        }
    }

    /* Build pair statistics and horizontal feasibility */
    for (int t = 0; t < MAX_ROWS; ++t) {
        const uint8_t *a = row_cells[t];
        for (int b = 0; b < MAX_ROWS; ++b) {
            const uint8_t *c = row_cells[b];

            /* d for column pair 0‑1 */
            uint8_t m0 = a[0];
            if (a[1] < m0) m0 = a[1];
            if (c[0] < m0) m0 = c[0];
            if (c[1] < m0) m0 = c[1];
            uint8_t M0 = a[0];
            if (a[1] > M0) M0 = a[1];
            if (c[0] > M0) M0 = c[0];
            if (c[1] > M0) M0 = c[1];
            pair_d0[t][b] = M0 - m0;

            /* d for column pair 1‑2 */
            uint8_t m1 = a[1];
            if (a[2] < m1) m1 = a[2];
            if (c[1] < m1) m1 = c[1];
            if (c[2] < m1) m1 = c[2];
            uint8_t M1 = a[1];
            if (a[2] > M1) M1 = a[2];
            if (c[1] > M1) M1 = c[1];
            if (c[2] > M1) M1 = c[2];
            pair_d1[t][b] = M1 - m1;

            /* horizontal constraint for the pair */
            valid_pair[t][b] =
                absdiff_uint8(pair_d0[t][b], pair_d1[t][b]) == 1;
        }
    }

    /* Enumerate valid states */
    state_count = 0;
    for (int t = 0; t < MAX_ROWS; ++t) {
        for (int b = 0; b < MAX_ROWS; ++b) {
            if (!valid_pair[t][b]) {
                state_index[t][b] = -1;
                continue;
            }
            state_index[t][b] = state_count;
            state_top[state_count]   = t;
            state_bot[state_count]   = b;
            state_d0 [state_count]   = pair_d0[t][b];
            state_d1 [state_count]   = pair_d1[t][b];
            ++state_count;
        }
    }

    /* Compute outgoing edges per state */
    for (int s = 0; s < state_count; ++s) out_count[s] = 0;
    for (int s1 = 0; s1 < state_count; ++s1) {
        int bot   = state_bot[s1];
        uint8_t d0a = state_d0[s1];
        uint8_t d1a = state_d1[s1];
        for (int s2 = 0; s2 < state_count; ++s2) {
            if (state_top[s2] != bot) continue;
            if (absdiff_uint8(d0a, state_d0[s2]) != 1) continue;
            if (absdiff_uint8(d1a, state_d1[s2]) != 1) continue;
            ++out_count[s1];
        }
    }

    /* Prefix sums for edge index starts */
    out_start[0] = 0;
    for (int s = 0; s < state_count; ++s)
        out_start[s + 1] = out_start[s] + out_count[s];

    /* Allocate and fill edges array */
    int total_edges = out_start[state_count];
    out_edges = (int*)malloc(total_edges * sizeof(int));

    /* Temporary array for insertion positions */
    int *pos = (int*)malloc(state_count * sizeof(int));
    memcpy(pos, out_start, state_count * sizeof(int));

    for (int s1 = 0; s1 < state_count; ++s1) {
        int bot   = state_bot[s1];
        uint8_t d0a = state_d0[s1];
        uint8_t d1a = state_d1[s1];
        for (int s2 = 0; s2 < state_count; ++s2) {
            if (state_top[s2] != bot) continue;
            if (absdiff_uint8(d0a, state_d0[s2]) != 1) continue;
            if (absdiff_uint8(d1a, state_d1[s2]) != 1) continue;
            out_edges[pos[s1]++] = s2;
        }
    }
    free(pos);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    init_precomputed();

    if (n == 0) return (seq_t)0; /* undefined, safe fallback */

    seq_t *dp_cur = dp0;
    seq_t *dp_nxt = dp1;

    /* Initialise dp_cur to 1 for each valid state (first pair) */
    for (int i = 0; i < state_count; ++i)
        dp_cur[i] = (seq_t)1;

    /* Number of vertical transitions is n-1 */
    for (idx_t step = 1; step < n; ++step) {
        memset(dp_nxt, 0, state_count * sizeof(seq_t));
        for (int s = 0; s < state_count; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;
            int start = out_start[s];
            int end   = out_start[s + 1];
            for (int e = start; e < end; ++e) {
                int t = out_edges[e];
                dp_nxt[t] += val;
            }
        }
        /* swap dp_cur and dp_nxt */
        seq_t *tmp = dp_cur;
        dp_cur = dp_nxt;
        dp_nxt = tmp;
    }

    /* Sum over all ending states */
    seq_t result = 0;
    for (int s = 0; s < state_count; ++s) result += dp_cur[s];
    return result;
}
