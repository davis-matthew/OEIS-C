/*
 * A205994  Number of (n+1)X5 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to two, and every 2X2
 *          determinant nonzero.
 * https://oeis.org/A205994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the (n+1)×5 arrays with entries 0..2 such that every
 *            2×2 subblock has exactly two clockwise edge increases and a
 *            non‑zero determinant. It is computed by dynamic programming over
 *            the 3^5 possible row patterns
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>


/* Compile‑time constants for the state space. */
#define STATES 243   /* 3^5 */
#define COLS   5

/* Pre‑computed digits of each state in base 3. */
static int row_digits[STATES][COLS];

/* Adjacency lists of allowed transitions for each state. */
static int  adj[STATES][STATES];
static unsigned char adj_len[STATES];

/* Flag to initialise the pre‑computation only once. */
static bool initialized = false;

/* Checks whether a transition from state `prev` to state `next`
   satisfies the two conditions for every 2×2 subblock. */
static inline bool allowed_transition(int prev, int next)
{
    for (int j = 0; j < COLS - 1; ++j) {
        int a = row_digits[prev][j];
        int b = row_digits[prev][j + 1];
        int c = row_digits[next][j];
        int d = row_digits[next][j + 1];

        unsigned cnt = 0;
        if (b > a) ++cnt;
        if (d > b) ++cnt;
        if (c > d) ++cnt;
        if (a > c) ++cnt;
        if (cnt != 2) return false;

        if (a * d - b * c == 0) return false;  /* determinant zero */
    }
    return true;
}

/* One‑time pre‑computation of row digits and adjacency lists. */
static void init_state_space(void)
{
    /* Convert each state index to its 5‑digit base‑3 representation. */
    for (int s = 0; s < STATES; ++s) {
        int v = s;
        for (int j = 0; j < COLS; ++j) {
            row_digits[s][j] = v % 3;
            v /= 3;
        }
    }

    /* Build adjacency lists for all transitions. */
    for (int i = 0; i < STATES; ++i) {
        adj_len[i] = 0;
        for (int j = 0; j < STATES; ++j) {
            if (allowed_transition(i, j)) {
                adj[i][adj_len[i]++] = j;
            }
        }
    }
    initialized = true;
}

/* Main sequence function required by the test harness. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1; return 0 for n = 0. */
    if (n == 0) return (seq_t)0;

    if (!initialized) init_state_space();

    /* DP tables for the current and next row counts. */
    seq_t dp[STATES];
    seq_t next[STATES];

    /* Initialisation: any first row is possible. */
    for (int i = 0; i < STATES; ++i) {
        dp[i] = (seq_t)1;
        next[i] = (seq_t)0;
    }

    /* Perform n transitions (n steps between the n+1 rows). */
    for (idx_t step = 1; step <= n; ++step) {
        /* Clear the next table. */
        for (int i = 0; i < STATES; ++i) next[i] = (seq_t)0;

        /* Accumulate counts via adjacency. */
        for (int i = 0; i < STATES; ++i) {
            seq_t val = dp[i];
            if (val == (seq_t)0) continue;
            unsigned char deg = adj_len[i];
            for (unsigned char k = 0; k < deg; ++k) {
                int j = adj[i][k];
                next[j] += val;
            }
        }

        /* Swap dp and next for the next iteration. */
        for (int i = 0; i < STATES; ++i) {
            dp[i] = next[i];
            next[i] = (seq_t)0;
        }
    }

    /* Total number of valid arrays is the sum over all final states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i) total += dp[i];
    return total;
}
