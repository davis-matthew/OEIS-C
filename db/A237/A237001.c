/*
 * A237001  Number of (n+1)X(2+1) 0..3 arrays with the upper median plus the
 *          lower median of every 2X2 subblock equal.
 * https://oeis.org/A237001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A237001
    Number of (n+1)×3 arrays with elements 0..3 whose upper‐median plus lower‐median of every
    2×2 subblock is the same (constant C).  This implementation uses a linear‑time
    dynamic programming over the rows of the array.  It is written in C11/C++17
    and uses the types `seq_t` and `idx_t` supplied by the build. */

#include <stdint.h>
#include <stdlib.h>

/* 3 columns, each 0..3 : 4³ = 64 possible row states   */
#define NUM_STATES 64
/* median‐sum values 0..6 for a 2×2 block                  */
#define MAX_C 7

/* precomputed data for the DP transitions */
static uint8_t mid_sum_arr[256];                      /* median‑sum for 4 values   */
static uint8_t adj_cnt[MAX_C][NUM_STATES];           /* number of neighbours      */
static uint8_t adj_next[MAX_C][NUM_STATES][NUM_STATES]; /* neighbour lists          */

/*--------------------------------------------------------------------*/
/* initialise adjacency lists – one time only                         */
static void init_adj(void)
{
    /* median‑sum of any 2×2 block */
    for (int idx = 0; idx < 256; ++idx) {
        int v0 = (idx >> 6)        & 3;
        int v1 = (idx >> 4)        & 3;
        int v2 = (idx >> 2)        & 3;
        int v3 =  idx              & 3;
        int mn = v0, mx = v0;
        if (v1 < mn) mn = v1; if (v1 > mx) mx = v1;
        if (v2 < mn) mn = v2; if (v2 > mx) mx = v2;
        if (v3 < mn) mn = v3; if (v3 > mx) mx = v3;
        mid_sum_arr[idx] = (uint8_t)(v0 + v1 + v2 + v3 - mn - mx);
    }

    /* build adjacency lists for each possible constant C */
    for (int r1 = 0; r1 < NUM_STATES; ++r1) {
        int a1 = (r1 >> 4) & 3;
        int b1 = (r1 >> 2) & 3;
        int c1 =  r1      & 3;
        for (int r2 = 0; r2 < NUM_STATES; ++r2) {
            int a2 = (r2 >> 4) & 3;
            int b2 = (r2 >> 2) & 3;
            int c2 =  r2       & 3;

            uint8_t idx_left  = (a1 << 6) | (b1 << 4) | (a2 << 2) | b2;
            uint8_t idx_right = (b1 << 6) | (c1 << 4) | (b2 << 2) | c2;
            uint8_t sum_left  = mid_sum_arr[idx_left];
            uint8_t sum_right = mid_sum_arr[idx_right];

            if (sum_left == sum_right) {
                uint8_t C  = sum_left;              /* 0..6 */
                uint8_t pos = adj_cnt[C][r1];
                adj_next[C][r1][pos] = (uint8_t)r2;
                adj_cnt[C][r1] = pos + 1;
            }
        }
    }
}

/*--------------------------------------------------------------------*/
/*    Vector table of size (MAX_C × NUM_STATES) holds counts per state   */
static void compute_a_internal(idx_t n, seq_t *out)
/*
   Computes a(n) by dynamic programming and writes the result into *out.
   No global or cached totals are kept – the function works independently
   of previous calls, satisfying requirement 6.
*/
{
    /* static dp tables (persist only across this function call) */
    seq_t dp_cur[MAX_C][NUM_STATES];
    seq_t dp_next[MAX_C][NUM_STATES];

    /* first row: any state is allowed – initialise to 1 for every C/state pair */
    for (int C = 0; C < MAX_C; ++C)
        for (int s = 0; s < NUM_STATES; ++s)
            dp_cur[C][s] = (seq_t)1;

    /* perform n transitions – each transition adds a new row to the array  */
    for (size_t step = 0; step < (size_t)n; ++step) {
        /* zero the next buffer */
        for (int C = 0; C < MAX_C; ++C)
            for (int s = 0; s < NUM_STATES; ++s)
                dp_next[C][s] = (seq_t)0;

        /* transition according to precomputed adjacency lists */
        for (int C = 0; C < MAX_C; ++C) {
            for (int i = 0; i < NUM_STATES; ++i) {
                seq_t val = dp_cur[C][i];
                if (val == 0) continue;
                uint8_t neigh = adj_cnt[C][i];
                for (uint8_t k = 0; k < neigh; ++k) {
                    uint8_t j = adj_next[C][i][k];
                    dp_next[C][j] += val;
                }
            }
        }

        /* copy next to current for the next step */
        for (int C = 0; C < MAX_C; ++C)
            for (int s = 0; s < NUM_STATES; ++s)
                dp_cur[C][s] = dp_next[C][s];
    }

    /* final result is the sum over all constant values C and terminal states */
    seq_t total = (seq_t)0;
    for (int C = 0; C < MAX_C; ++C)
        for (int s = 0; s < NUM_STATES; ++s)
            total += dp_cur[C][s];
    *out = total;
}

/*--------------------------------------------------------------------*/
/* Main entry point required by the harness                         */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;      /* behaviour below offset is irrelevant */

    /* initialise adjacency structures once */
    static int adj_initialized = 0;
    if (!adj_initialized) {
        init_adj();
        adj_initialized = 1;
    }

    seq_t result;
    compute_a_internal(n, &result);
    return result;
}
