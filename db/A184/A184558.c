/*
 * A184558  Number of (n+1)X4 0..2 arrays with no 2X2 subblock summing to less
 *          than 4.
 * https://oeis.org/A184558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Counts the number of (n+1)×4 arrays with entries 0..2 in which every
 * 2×2 subblock has sum at least 4.  The method is a standard
 * transfer‑matrix dynamic programming: each row is a state (81
 * possibilities).  An edge (i→j) exists iff the two consecutive rows
 * i,j satisfy the 2×2‑sum condition.  The number of length‑(n+1) rows
 * equals the total number of walks of length n in this directed graph.
 */

#include <stdint.h>
#include <stdbool.h>

#define STATE_COUNT 81

/* Convert a state number to its 4 base‑3 digits. */
static void state_digits(int s, int d[4]) {
    d[3] = s % 3; s /= 3;
    d[2] = s % 3; s /= 3;
    d[1] = s % 3; s /= 3;
    d[0] = s;
}

/* Build the 81×81 adjacency matrix.  A[i][j]==true iff rows i and j
 * form an allowed 2×2 block. */
static void init_adjacent(bool adj[STATE_COUNT][STATE_COUNT]) {
    int di[4], dj[4];
    for (int i = 0; i < STATE_COUNT; ++i) {
        state_digits(i, di);
        for (int j = 0; j < STATE_COUNT; ++j) {
            state_digits(j, dj);
            bool ok = true;
            for (int k = 0; k < 3; ++k) {
                int sum = di[k] + di[k + 1] + dj[k] + dj[k + 1];
                if (sum < 4) { ok = false; break; }
            }
            adj[i][j] = ok;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n) {
    /* One‑time adjacency table initialisation. */
    static bool adj[STATE_COUNT][STATE_COUNT];
    static bool ready = false;
    if (!ready) { init_adjacent(adj); ready = true; }

    /* DP arrays: cur[i] = #ways to reach state i after current number
     * of rows; nxt holds the next iteration. */
    seq_t cur[STATE_COUNT];
    seq_t nxt[STATE_COUNT];

    /* Start with a single row – any of the 81 states is possible. */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 1;

    /* Produce (n+1) rows → n transitions. */
    for (idx_t step = 0; step < n; ++step) {
        for (int j = 0; j < STATE_COUNT; ++j) nxt[j] = 0;
        for (int i = 0; i < STATE_COUNT; ++i) {
            if (cur[i] == 0) continue;           /* skip empty column */
            for (int j = 0; j < STATE_COUNT; ++j) {
                if (adj[i][j]) nxt[j] += cur[i];
            }
        }
        for (int i = 0; i < STATE_COUNT; ++i) cur[i] = nxt[i];
    }

    /* Sum all end states to obtain the total number of arrays. */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += cur[i];
    return total;
}
