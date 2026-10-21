/*
 * A206879  Number of n X 4 0..1 arrays avoiding 0 1 0 horizontally and 1 0 1
 *          vertically.
 * https://oeis.org/A206879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×4 binary arrays avoiding a horizontal 0‑1‑0
 *            and a vertical 1‑0‑1 pattern. The algorithm uses dynamic
 *            programming over the last two rows. There are 12 admissible row
 *            patterns (the patterns that intrinsically avoid horizontal
 *            0‑1‑0). A transition from rows (i,j) to k is valid iff...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* The types seq_t and idx_t are provided by the build system. */

static const idx_t PAT[12] = {
    0, 1, 3, 6, 7, 8, 9, 11, 12, 13, 14, 15
};

/* Transition table: transitions[i][j][k] == true if rows
   PAT[i] followed by PAT[j] followed by PAT[k]
   do not contain a vertical 1‑0‑1 pattern.
   The table is filled lazily on first call. */
static bool transitions[12][12][12] = {0};
static bool transitions_computed = false;

static void compute_transitions(void)
{
    if (transitions_computed) return;
    for (idx_t i = 0; i < 12; ++i) {
        for (idx_t j = 0; j < 12; ++j) {
            for (idx_t k = 0; k < 12; ++k) {
                bool ok = true;
                for (idx_t pos = 0; pos < 4 && ok; ++pos) {
                    int b1 = (PAT[i] >> pos) & 1;
                    int b2 = (PAT[j] >> pos) & 1;
                    int b3 = (PAT[k] >> pos) & 1;
                    if (b1 == 1 && b2 == 0 && b3 == 1) ok = false;
                }
                transitions[i][j][k] = ok;
            }
        }
    }
    transitions_computed = true;
}

seq_t a(idx_t n)
{
    /* a(1) = 12.  For n >= 2 the DP below works correctly. */
    if (n == 1) return (seq_t)12;

    compute_transitions();

    /* DP tables: cur[i][j] = number of ways to end with rows (i,j)
       for the current sequence length. */
    seq_t cur[12][12];
    seq_t nxt[12][12];

    /* Initialization for length 2: each pair of admissible rows
       occurs exactly once. */
    for (idx_t i = 0; i < 12; ++i)
        for (idx_t j = 0; j < 12; ++j)
            cur[i][j] = (seq_t)1;

    /* Extend the sequences to length n.  For n==2 the loop is skipped. */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero next table. */
        for (idx_t i = 0; i < 12; ++i)
            for (idx_t j = 0; j < 12; ++j)
                nxt[i][j] = (seq_t)0;

        /* Transition step. */
        for (idx_t i = 0; i < 12; ++i) {
            for (idx_t j = 0; j < 12; ++j) {
                seq_t ways = cur[i][j];
                if (!ways) continue;
                for (idx_t k = 0; k < 12; ++k) {
                    if (transitions[i][j][k]) {
                        nxt[j][k] = nxt[j][k] + ways;
                    }
                }
            }
        }

        /* Prepare for next iteration. */
        for (idx_t i = 0; i < 12; ++i)
            for (idx_t j = 0; j < 12; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Sum all ending states to obtain a(n). */
    seq_t total = (seq_t)0;
    for (idx_t i = 0; i < 12; ++i)
        for (idx_t j = 0; j < 12; ++j)
            total = total + cur[i][j];

    return total;
}
