/*
 * A232259  Number of (2+1) X (n+1) 0..2 arrays with every element both >= and
 *          <= some horizontal or antidiagonal neighbor.
 * https://oeis.org/A232259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A232259: Number of (2+1) × (n+1) 0..2 arrays with every element
 * having at least one neighbor ≤ it and at least one neighbor ≥ it,
 * where neighbors are horizontal (same row, adjacent column) or
 * anti‑diagonal (row changes by ±1 while column changes by ∓1).
 *
 * This implementation uses a transfer‑matrix dynamic programming
 * over column states (27 possible 3‑digit base‑3 vectors) plus a
 * sentinel value for missing neighbours.  For each middle column
 * we precompute a 28×27×28 validity table, then iterate forward
 * over the n+1 columns.  Each transition checks the constraints
 * for the current column using its left and right neighbours.
 * Complexity: O(n·28·27²) ≈ 2·10⁴ operations per column, well
 * below the limits for n ≈ 10 000.  The function works for any
 * integer type seq_t, including arbitrary‑precision types. */

#include <stddef.h>
#include <stdbool.h>

/* A helper that checks whether a column state `c` satisfies the
   neighbour constraints when its left neighbour is `p` and its
   right neighbour is `n`.  `sent` is the sentinel value used for
   a missing neighbour.  */  
static bool valid_transition(int p, int c, int n, int sent)
{
    int cv[3], pv[3] = {0, 0, 0}, nv[3] = {0, 0, 0};

    /* Decode base‑3 column states. */
    cv[0] = c % 3;
    cv[1] = (c / 3) % 3;
    cv[2] = c / 9;

    if (p != sent) {
        pv[0] = p % 3;
        pv[1] = (p / 3) % 3;
        pv[2] = p / 9;
    }
    if (n != sent) {
        nv[0] = n % 3;
        nv[1] = (n / 3) % 3;
        nv[2] = n / 9;
    }

    for (int r = 0; r < 3; ++r) {
        int cur = cv[r];
        bool has_low  = false;
        bool has_high = false;

        /* Horizontal left neighbour (same row, previous column). */
        if (p != sent) {
            int v = pv[r];
            if (v <= cur) has_low  = true;
            if (v >= cur) has_high = true;
        }

        /* Anti‑diagonal left neighbour (row +1, previous column). */
        if (p != sent && r < 2) {
            int v = pv[r + 1];
            if (v <= cur) has_low  = true;
            if (v >= cur) has_high = true;
        }

        /* Horizontal right neighbour (same row, next column). */
        if (n != sent) {
            int v = nv[r];
            if (v <= cur) has_low  = true;
            if (v >= cur) has_high = true;
        }

        /* Anti‑diagonal right neighbour (row -1, next column). */
        if (n != sent && r > 0) {
            int v = nv[r - 1];
            if (v <= cur) has_low  = true;
            if (v >= cur) has_high = true;
        }

        if (!has_low || !has_high)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    const int SENT = 27;                /* sentinel for missing neighbour */
    const size_t COLS  = (size_t)n + 1; /* number of columns in the array */
    const size_t STATES = 28;           /* 0..26 real states, 27 sentinel    */

    /* Pre‑compute the valid transition table once. */
    static bool init = false;
    static bool valid[28][27][28];
    if (!init) {
        for (int p = 0; p < (int)STATES; ++p) {
            for (int c = 0; c < 27; ++c) {
                for (int nn = 0; nn < 27; ++nn)
                    valid[p][c][nn] = valid_transition(p, c, nn, SENT);
                valid[p][c][SENT] = valid_transition(p, c, SENT, SENT);
            }
        }
        /* Transitions involving the sentinel as the middle state are
           never needed (there is no real column with state SENT), but
           leave them zero or unset; they won't be accessed. */
        init = true;
    }

    /* Dynamic programming tables. */
    static seq_t counts[28 * 27];
    static seq_t next_counts[28 * 27];
    size_t total = STATES * 27;

    /* Zero all entries. */
    for (size_t i = 0; i < total; ++i) {
        counts[i] = 0;
        next_counts[i] = 0;
    }

    /* Initialise first column: previous neighbour is missing. */
    for (int c = 0; c < 27; ++c)
        counts[SENT * 27 + c] = 1;

    /* Process remaining columns. */
    for (size_t step = 1; step < COLS; ++step) {
        /* Zero next_counts. */
        for (size_t i = 0; i < total; ++i)
            next_counts[i] = 0;

        for (int p = 0; p < (int)STATES; ++p) {
            for (int c = 0; c < 27; ++c) {
                seq_t cur = counts[p * 27 + c];
                if (cur == 0) continue;
                for (int nn = 0; nn < 27; ++nn) {
                    if (!valid[p][c][nn]) continue;
                    next_counts[c * 27 + nn] += cur;
                }
            }
        }

        /* Move to next step. */
        for (size_t i = 0; i < total; ++i)
            counts[i] = next_counts[i];
    }

    /* Final check for the last column (right neighbour is missing). */
    seq_t ans = 0;
    for (int p = 0; p < (int)STATES; ++p) {
        for (int c = 0; c < 27; ++c) {
            seq_t cur = counts[p * 27 + c];
            if (cur == 0) continue;
            if (valid[p][c][SENT])
                ans += cur;
        }
    }
    return ans;
}
