/*
 * A233943  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          having the sum of the absolute values of the edge differences
 *          equal to 4.
 * https://oeis.org/A233943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays over {0,1,2} * such that every
 *            2×2 subblock has edge‑difference sum 4. * * Computation: There
 *            are 3³=27 possible row patterns. * Build an adjacency matrix
 *            `adj[s][t]` that is true iff * row pattern `s` can be followed
 *            by row pattern `t`. * A sequence of n+1 rows co...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Forward declaration of the function to be produced. */
seq_t a(idx_t n);

/* Number of possible row states (3^3). */
#define STATE_NUM 27

/* The adjacency matrix as 0/1 bytes. */
static unsigned char adj[STATE_NUM][STATE_NUM];

/* Flag to check if the adjacency matrix has been built. */
static bool adj_built = false;

/* Decode a state index into its three digits (base‑3 digits). */
static void decode_state(idx_t state, int out[3])
{
    int tmp = (int)state;
    out[0] = tmp % 3;
    tmp /= 3;
    out[1] = tmp % 3;
    tmp /= 3;
    out[2] = tmp % 3;
}

/* Test compatibility of two row patterns. */
static bool compatible(idx_t s, idx_t t)
{
    int rs[3], ts[3];
    decode_state(s, rs);
    decode_state(t, ts);

    for (int j = 0; j < 2; ++j) {
        int sum = abs(rs[j] - ts[j])          /* left edge          */
                + abs(rs[j+1] - ts[j+1])      /* right edge         */
                + abs(rs[j] - rs[j+1])        /* top horizontal     */
                + abs(ts[j] - ts[j+1]);       /* bottom horizontal  */
        if (sum != 4) return false;
    }
    return true;
}

/* Build the adjacency matrix once. */
static void build_adj(void)
{
    for (idx_t s = 0; s < STATE_NUM; ++s) {
        for (idx_t t = 0; t < STATE_NUM; ++t) {
            adj[s][t] = compatible(s, t) ? 1 : 0;
        }
    }
    adj_built = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; values below 1 are undefined */

    if (!adj_built) build_adj();

    /* Current DP vector: counts for each state.  Initialize to 1 for all 27 states. */
    seq_t cur[STATE_NUM];
    seq_t nxt[STATE_NUM];

    for (int i = 0; i < STATE_NUM; ++i) cur[i] = (seq_t)1;

    /* Perform n transitions (n steps between n+1 rows). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Zero out next vector. */
        for (int i = 0; i < STATE_NUM; ++i) nxt[i] = (seq_t)0;

        /* Transition: cur[s] -> nxt[t] if adj[s][t] */
        for (int s = 0; s < STATE_NUM; ++s) {
            if (cur[s] == (seq_t)0) continue;
            for (int t = 0; t < STATE_NUM; ++t) {
                if (adj[s][t]) {
                    nxt[t] += cur[s];
                }
            }
        }

        /* Copy nxt back to cur for the next iteration. */
        for (int i = 0; i < STATE_NUM; ++i) cur[i] = nxt[i];
    }

    /* Sum all counts to get a(n). */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATE_NUM; ++i) total += cur[i];
    return total;
}
