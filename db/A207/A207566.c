/*
 * A207566  Number of 5Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by a transfer‑matrix DP over 5×n binary matrices
 *            avoiding horizontal 001/010 and vertical 011/110 patterns. No
 *            closed form known
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Precomputed 5‑bit columns that satisfy the vertical constraints
   (i.e. avoid the triples 011 and 110). */
static const unsigned char GOOD[14] = {
    0x00, 0x01, 0x02, 0x04, 0x05,
    0x08, 0x09, 0x0A, 0x10, 0x11,
    0x12, 0x14, 0x15, 0x1F
};
#define NGOOD 14
#define STATE_COUNT (NGOOD * NGOOD)

/* Test whether a 5‑bit pattern satisfies the vertical restriction */
static bool vertical_ok(unsigned col)
{
    for (int r = 0; r <= 2; ++r) {
        int a = (col >> r) & 1;
        int b = (col >> (r + 1)) & 1;
        int c = (col >> (r + 2)) & 1;
        if ((a == 0 && b == 1 && c == 1) ||
            (a == 1 && b == 1 && c == 0))
            return false;
    }
    return true;
}

/* Test horizontal restriction for three consecutive columns */
static bool horizontal_ok(unsigned a, unsigned b, unsigned c)
{
    for (int r = 0; r < 5; ++r) {
        int x = (a >> r) & 1;
        int y = (b >> r) & 1;
        int z = (c >> r) & 1;
        if ((x == 0 && y == 0 && z == 1) ||
            (x == 0 && y == 1 && z == 0))
            return false;
    }
    return true;
}

/* Transfer matrix: for each state (i,j) list of next states (j,k). */
static unsigned char next_state[STATE_COUNT][NGOOD];
static unsigned char next_cnt[STATE_COUNT];
static bool initialized = false;

/* The main sequence function */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 0;                 /* offset is 1, behavior below is irrelevant */
    if (n == 1) return (seq_t)14;
    if (n == 2) return (seq_t)196;

    /* Initialize transition table once */
    if (!initialized) {
        for (int i = 0; i < NGOOD; ++i) {
            for (int j = 0; j < NGOOD; ++j) {
                int state = i * NGOOD + j;
                next_cnt[state] = 0;
                for (int k = 0; k < NGOOD; ++k) {
                    if (horizontal_ok(GOOD[i], GOOD[j], GOOD[k])) {
                        next_state[state][next_cnt[state]++] = j * NGOOD + k;
                    }
                }
            }
        }
        initialized = true;
    }

    /* DP arrays */
    seq_t dp[STATE_COUNT];
    seq_t newdp[STATE_COUNT];
    for (int s = 0; s < STATE_COUNT; ++s) {
        dp[s]    = (seq_t)0;
        newdp[s] = (seq_t)0;
    }

    /* After two columns: each pair of good columns is valid once */
    for (int s = 0; s < STATE_COUNT; ++s) {
        dp[s] = (seq_t)1;
    }

    /* Process columns 3 .. n */
    for (idx_t col = 3; col <= n; ++col) {
        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t cnt = dp[s];
            if (cnt == (seq_t)0) continue;
            unsigned char *ns = next_state[s];
            unsigned char   nc = next_cnt[s];
            for (unsigned char t = 0; t < nc; ++t) {
                newdp[ns[t]] += cnt;
            }
        }
        /* swap dp and newdp */
        for (int s = 0; s < STATE_COUNT; ++s) {
            dp[s] = newdp[s];
            newdp[s] = (seq_t)0;
        }
    }

    /* Sum all final states */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        total += dp[s];
    }
    return total;
}
