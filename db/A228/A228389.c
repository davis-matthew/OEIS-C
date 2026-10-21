/*
 * A228389  Number of n X 7 binary arrays with top left value 1 and no two 1's
 *          adjacent horizontally or vertically.
 * https://oeis.org/A228389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over valid row masks (transfer matrix) */
/* A228389: number of n×7 binary arrays with top‑left 1 and no two 1s
 * adjacent horizontally or vertically.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Maximum number of columns.  */
#define MAX_COLS 7
#define MAX_MASK (1 << MAX_COLS)   /* 128 */

/* Global state for the transfer matrix (computed once). */
static bool ready = false;
static int  S = 0;                 /* number of valid row masks          */
static int  states[MAX_MASK];     /* valid masks                        */
static bool trans[MAX_MASK][MAX_MASK]; /* vertical adjacency forbidden  */
static seq_t cur[MAX_MASK], next[MAX_MASK];

/* One‑time initialization of states and transition matrix. */
static void init_tables(void)
{
    if (ready) return;

    /* Find all masks with no horizontal adjacency. */
    for (int m = 0; m < MAX_MASK; ++m) {
        if ((m & (m << 1)) == 0) {
            states[S++] = m;
        }
    }

    /* Vertical adjacency: transitions only if masks share no 1s. */
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            trans[i][j] = (states[i] & states[j]) == 0;
        }
    }

    ready = true;
}

/* Compute a(n).  The routine recomputes the DP from scratch, so it
 * works for any order of calls.  Complexity is O(n·S²) with
 * S ≤ 34. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n < 1 */

    init_tables();

    /* Initialise cur for first row (top‑left cell fixed to 1). */
    for (int i = 0; i < S; ++i) {
        cur[i] = (states[i] & 1) ? (seq_t)1 : (seq_t)0;
    }

    /* DP for subsequent rows. */
    for (idx_t row = 2; row <= n; ++row) {
        /* Zero next array. */
        for (int i = 0; i < S; ++i) next[i] = 0;

        /* Transition from every previous mask to every allowed next mask. */
        for (int i = 0; i < S; ++i) {
            seq_t ways = cur[i];
            if (ways == 0) continue;
            for (int j = 0; j < S; ++j) {
                if (!trans[i][j]) continue;
                next[j] += ways;
            }
        }

        /* Swap next into cur. */
        for (int i = 0; i < S; ++i) cur[i] = next[i];
    }

    /* Sum all end states to obtain a(n). */
    seq_t total = 0;
    for (int i = 0; i < S; ++i) total += cur[i];
    return total;
}
