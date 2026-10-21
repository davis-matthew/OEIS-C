/*
 * A207911  Number of 6Xn 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 0 0 and 0 1 1 vertically.
 * https://oeis.org/A207911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: dynamic programming over columns for 6×n binary arrays
 * avoiding horizontal patterns 000 and 010 and vertical patterns
 * 000 and 011.  The algorithm enumerates all admissible column
 * masks, builds a transition table for triples of columns, and then
 * iteratively counts arrays of length n.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool initialized = false;
static int  col_mask[64];
static int  col_map[64];
static int  col_count;              /* number of admissible column masks */
static int  trans[18][18][18];      /* transition: 1 if (p,q,r) is allowed */
static int  pair_count;             /* col_count * col_count */

/* Initialize lookup tables the first time a() is called. */
static void init(void)
{
    if (initialized) return;
    initialized = true;

    /* Build the list of admissible column masks. */
    for (int m = 0; m < 64; ++m) col_map[m] = -1;
    col_count = 0;
    for (int m = 0; m < 64; ++m) {
        bool ok = true;
        for (int i = 0; i <= 3; ++i) {            /* vertical triples */
            int a = (m >> i) & 1;
            int b = (m >> (i + 1)) & 1;
            int c = (m >> (i + 2)) & 1;
            if ((a == 0 && b == 0 && c == 0) ||
                (a == 0 && b == 1 && c == 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            col_mask[col_count] = m;
            col_map[m] = col_count;
            ++col_count;
        }
    }

    /* Transition table for horizontal constraints. */
    for (int p = 0; p < col_count; ++p) {
        for (int q = 0; q < col_count; ++q) {
            for (int r = 0; r < col_count; ++r) {
                bool ok = true;
                for (int i = 0; i < 6; ++i) {
                    int b0 = (col_mask[p] >> i) & 1;
                    int b1 = (col_mask[q] >> i) & 1;
                    int b2 = (col_mask[r] >> i) & 1;
                    if ((b0 == 0 && b1 == 0 && b2 == 0) ||
                        (b0 == 0 && b1 == 1 && b2 == 0)) {
                        ok = false;
                        break;
                    }
                }
                trans[p][q][r] = ok ? 1 : 0;
            }
        }
    }

    pair_count = col_count * col_count;
}

/* --------------------------------------------------------------------- */
/* Main function required by the test harness.                           */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    init();

    if (n == 1) return (seq_t)col_count;
    if (n == 2) return (seq_t)col_count * (seq_t)col_count;

    /* DP arrays on the stack.  We know pair_count <= 360 for this sequence. */
    seq_t dp_prev[360];
    seq_t dp_next[360];
    size_t pc = (size_t)pair_count;

    /* initialise counts for length 2: all pairs have weight 1 */
    for (size_t i = 0; i < pc; ++i) dp_prev[i] = (seq_t)1;

    /* iterate over column positions 3 .. n */
    for (idx_t len = 3; len <= n; ++len) {
        /* zero the next array */
        for (size_t i = 0; i < pc; ++i) dp_next[i] = (seq_t)0;

        /* transition */
        for (size_t idx = 0; idx < pc; ++idx) {
            seq_t val = dp_prev[idx];
            if (val == (seq_t)0) continue;
            int p = (int)(idx / col_count);
            int q = (int)(idx % col_count);
            for (int r = 0; r < col_count; ++r) {
                if (trans[p][q][r]) {
                    size_t next_idx = (size_t)q * col_count + r;
                    dp_next[next_idx] += val;
                }
            }
        }

        /* copy back for next step */
        for (size_t i = 0; i < pc; ++i) dp_prev[i] = dp_next[i];
    }

    /* sum over all end states to get a(n) */
    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < pc; ++i) sum += dp_prev[i];
    return sum;
}
