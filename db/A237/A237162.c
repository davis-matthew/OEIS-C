/*
 * A237162  Number of (n+1)X(3+1) 0..2 arrays with the maximum plus the
 *          minimum minus the upper median minus the lower median of every 2X2
 *          subblock equal.
 * https://oeis.org/A237162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A237162: Number of (n+1) × 4 arrays over {0,1,2} such that for every
   2 × 2 subblock the expression (max + min – upper median – lower median)
   has the same value in all subblocks.  a(1) = 777, a(2) = 4365, …  */
#include <stddef.h>
#include <stdbool.h>

#define STATES 81    /* 3^4 possible rows */
#define FACES  5     /* possible f-values: -2,-1,0,1,2 */

/* Helper to decode a state (0..80) into its 4 digits of base 3 */
static void decode_state(int state, int d[4]) {
    d[3] = state % 3; state /= 3;
    d[2] = state % 3; state /= 3;
    d[1] = state % 3; state /= 3;
    d[0] = state;
}

/* Compute f for a single 2x2 block given the four digit values.
   Returns the value f (-2..2). */
static int block_f(int a, int b, int c, int d) {
    int v[4] = {a,b,c,d};
    /* simple bubble sort of 4 elements */
    if (v[0] > v[1]) { int t=v[0]; v[0]=v[1]; v[1]=t; }
    if (v[2] > v[3]) { int t=v[2]; v[2]=v[3]; v[3]=t; }
    if (v[0] > v[2]) { int t=v[0]; v[0]=v[2]; v[2]=t; }
    if (v[1] > v[3]) { int t=v[1]; v[1]=v[3]; v[3]=t; }
    if (v[1] > v[2]) { int t=v[1]; v[1]=v[2]; v[2]=t; }
    return v[3] + v[0] - v[2] - v[1];
}

/* Precomputed data shared across all calls to a() */
static bool init_done = false;
static signed char pair_f[STATES][STATES];   /* -3 invalid, else 0..4 */
static int row_vals[STATES][4];

static void precompute(void) {
    if (init_done) return;
    int s;
    for (s = 0; s < STATES; ++s) decode_state(s, row_vals[s]);
    for (int p = 0; p < STATES; ++p) {
        for (int c = 0; c < STATES; ++c) {
            int fval = -3; /* invalid */
            int f0 = block_f(row_vals[p][0], row_vals[p][1], row_vals[c][0], row_vals[c][1]);
            int f1 = block_f(row_vals[p][1], row_vals[p][2], row_vals[c][1], row_vals[c][2]);
            int f2 = block_f(row_vals[p][2], row_vals[p][3], row_vals[c][2], row_vals[c][3]);
            if (f0 == f1 && f1 == f2) fval = f0 + 2; /* shift to 0..4 */
            pair_f[p][c] = (signed char)fval;
        }
    }
    init_done = true;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    precompute();

    idx_t rows = n + 1;          /* number of rows in the array */
    seq_t dp[STATES][FACES] = {0};
    seq_t ndp[STATES][FACES];

    /* initialization: after the first two rows */
    for (int prev = 0; prev < STATES; ++prev) {
        for (int curr = 0; curr < STATES; ++curr) {
            signed char fidx = pair_f[prev][curr];
            if (fidx >= 0) dp[curr][fidx] += (seq_t)1;
        }
    }

    if (rows == 2) { /* only two rows exist */
        seq_t total = (seq_t)0;
        for (int s = 0; s < STATES; ++s)
            for (int f = 0; f < FACES; ++f)
                total += dp[s][f];
        return total;
    }

    idx_t add_rows = rows - 2;    /* number of additional rows to add */
    for (idx_t step = 0; step < add_rows; ++step) {
        /* zero ndp */
        for (int s = 0; s < STATES; ++s)
            for (int f = 0; f < FACES; ++f)
                ndp[s][f] = (seq_t)0;

        for (int prev = 0; prev < STATES; ++prev) {
            for (int curr = 0; curr < STATES; ++curr) {
                signed char fidx = pair_f[prev][curr];
                if (fidx >= 0) {
                    ndp[curr][fidx] += dp[prev][fidx];
                }
            }
        }
        /* copy ndp to dp for next iteration */
        for (int s = 0; s < STATES; ++s)
            for (int f = 0; f < FACES; ++f)
                dp[s][f] = ndp[s][f];
    }

    /* final total */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATES; ++s)
        for (int f = 0; f < FACES; ++f)
            total += dp[s][f];
    return total;
}
