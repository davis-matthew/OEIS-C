/*
 * A206050  Number of (n+1)X5 0..2 arrays with the number of clockwise edge
 *          increases in every 2X2 subblock equal to one.
 * https://oeis.org/A206050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A206050: Number of (n+1)×5 arrays with entries 0..2 such that
   every 2×2 subblock has exactly one clockwise edge increase.
   Computation by dynamic programming on rows: each transition from one
   row to the next keeps only those pairs of rows that satisfy the
   per‑block condition.
   The implementation precomputes the allowed row transitions once
   and then iteratively applies them for the requested value of n.
*/
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* There are 3^5 = 243 possible rows of length 5 over {0,1,2}. */
#define STATE_COUNT 243

/* Precomputed mapping from state index to digits (0,1,2) per column. */
static unsigned char state_digits[STATE_COUNT][5];

/* Transition adjacency: for each state i, a list of states j that can
   follow i.  The list is stored consecutively; neighbor_count[i]
   records the number of neighbours.  2 bytes per pair (unsigned short). */
static unsigned short neighbors[STATE_COUNT][STATE_COUNT];
static unsigned char neighbor_count[STATE_COUNT];

/* Build the transition graph once. */
static void build_graph(void)
{
    /* Convert each state to its base‑3 representation. */
    for (unsigned int s = 0; s < STATE_COUNT; ++s) {
        unsigned int t = s;
        for (int col = 4; col >= 0; --col) {
            state_digits[s][col] = (unsigned char)(t % 3);
            t /= 3;
        }
    }

    /* For each pair (i,j) determine if j may follow i. */
    for (unsigned int i = 0; i < STATE_COUNT; ++i) {
        unsigned char nb = 0;
        for (unsigned int j = 0; j < STATE_COUNT; ++j) {
            bool ok = true;
            for (int col = 0; col < 4; ++col) {
                unsigned char a = state_digits[i][col];
                unsigned char b = state_digits[i][col + 1];
                unsigned char c = state_digits[j][col];
                unsigned char d = state_digits[j][col + 1];

                int inc = 0;
                if (b > a) ++inc;   /* a -> b */
                if (d > b) ++inc;   /* b -> d */
                if (c > d) ++inc;   /* d -> c */
                if (a > c) ++inc;   /* c -> a */

                if (inc != 1) { ok = false; break; }
            }
            if (ok) {
                neighbors[i][nb++] = (unsigned short)j;
            }
        }
        neighbor_count[i] = nb;
    }
}

/* Static DP buffers used for every call.  They are allocated on first use. */
static seq_t *dp      = NULL;
static seq_t *dp_next = NULL;
static bool graph_built = false;

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Lazily build graph and buffers. */
    if (!graph_built) {
        dp      = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
        dp_next = (seq_t *)malloc(sizeof(seq_t) * STATE_COUNT);
        build_graph();
        graph_built = true;
    }

    /* Start with any first row: one way for each state. */
    for (unsigned int s = 0; s < STATE_COUNT; ++s) dp[s] = 1;

    /* Perform n transitions to build an (n+1)×5 array. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero the next-layer buffer. */
        for (unsigned int s = 0; s < STATE_COUNT; ++s) dp_next[s] = 0;

        /* Propagate counts along allowed transitions. */
        for (unsigned int i = 0; i < STATE_COUNT; ++i) {
            seq_t ways = dp[i];
            if (!ways) continue;
            unsigned char nb = neighbor_count[i];
            for (unsigned char k = 0; k < nb; ++k) {
                unsigned short j = neighbors[i][k];
                dp_next[j] += ways;
            }
        }

        /* Swap current and next buffers for next iteration. */
        seq_t *tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    /* Sum over final-row states to obtain the total number of arrays. */
    seq_t total = 0;
    for (unsigned int s = 0; s < STATE_COUNT; ++s)
        total += dp[s];
    return total;
}
