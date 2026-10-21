/*
 * A236005  Number of (n+1)X(3+1) 0..3 arrays with the minimum plus the
 *          maximum equal to the lower median plus the upper median in every
 *          2X2 subblock.
 * https://oeis.org/A236005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: Dynamic programming over row states.
   Sequence A236005: number of (n+1)×4 arrays with values 0..3 such that
   in every 2×2 subblock, min + max = lower median + upper median. The
   DP enumerates all 256 possible rows (4 digits base‑4) and counts
   all walks of length n (i.e. n transitions between consecutive rows) in
   the valid‑transition graph.  The result is the sum of all walk
   counts of length n.  Time: O(n·E) with E≈3000 (<256²).  Memory: O(256). */

#include <stdbool.h>
#include <stddef.h>

/* Forward declaration of helper to build adjacency graph once. */
static void build_graph(void);

/* Global data used by the DP. */
static bool graph_built = false;
static unsigned char row_digits[256][4];          /* 0..3 digits for each row */
static unsigned short adj_deg[256];              /* number of outgoing edges for each row */
static unsigned char adj_next[256][256];          /* next state lists (max 256 per row) */

/* Helper to test whether a 2×2 block satisfies the condition. */
static bool block_valid(int a, int b, int c, int d)
{
    /* Sort the four values (simple bubble sort for 4 items). */
    int v[4] = {a, b, c, d};
    /* 3 passes of bubble sort */
    for (int i = 0; i < 4; ++i) {
        for (int j = i + 1; j < 4; ++j) {
            if (v[i] > v[j]) {
                int t = v[i];
                v[i] = v[j];
                v[j] = t;
            }
        }
    }
    return v[0] + v[3] == v[1] + v[2];
}

/* Build the transition graph between rows once. */
static void build_graph(void)
{
    /* Precompute digits for each row index (0..255). */
    for (int idx = 0; idx < 256; ++idx) {
        unsigned int val = (unsigned int)idx;
        for (int pos = 0; pos < 4; ++pos) {
            row_digits[idx][pos] = (unsigned char)(val & 3);  /* least significant = col0 */
            val >>= 2;
        }
    }

    /* Compute adjacency. */
    for (int i = 0; i < 256; ++i) {
        unsigned short deg = 0;
        for (int j = 0; j < 256; ++j) {
            bool ok = true;
            /* For the 3 overlapping 2×2 blocks. */
            for (int col = 0; col < 3; ++col) {
                int a = row_digits[i][col];
                int b = row_digits[i][col + 1];
                int c = row_digits[j][col];
                int d = row_digits[j][col + 1];
                if (!block_valid(a, b, c, d)) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                adj_next[i][deg++] = (unsigned char)j;
            }
        }
        adj_deg[i] = deg;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (!graph_built) {
        build_graph();
        graph_built = true;
    }

    /* Temporary vectors for DP. */
    static seq_t curr[256];
    static seq_t next[256];
    const seq_t one = (seq_t)1;
    const seq_t zero = (seq_t)0;

    /* Initialize curr to all 1s (every row possible at start). */
    for (int i = 0; i < 256; ++i) {
        curr[i] = one;
    }

    /* Perform n transitions (since array has n+1 rows). */
    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 256; ++i) {
            next[i] = zero;
        }
        for (int i = 0; i < 256; ++i) {
            if (curr[i] == zero) continue;
            unsigned short deg = adj_deg[i];
            for (unsigned short k = 0; k < deg; ++k) {
                int j = adj_next[i][k];
                next[j] += curr[i];
            }
        }
        /* Swap curr and next. */
        for (int i = 0; i < 256; ++i) {
            curr[i] = next[i];
        }
    }

    /* Sum all ending states to get total number of arrays. */
    seq_t total = zero;
    for (int i = 0; i < 256; ++i) {
        total += curr[i];
    }
    return total;
}
