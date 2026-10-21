/*
 * A207406  Number of 6 X n 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is computed via a transfer‑matrix DP counting 6×n binary
 *            arrays that avoid horizontal patterns 001 and 011 and vertical
 *            patterns 001 and 110. The DP uses 12 allowed 6‑bit columns
 *            (vertical safe) and states that are ordered pairs of columns
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 208, u128 n <= 210,
 *            big any n
 * Helpers:   vertical_safe
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/vertical_safe.h"

#define VMAX 64
#define SMAX 144  /* V*V where V=12 */

static int V = 0;                 /* number of vertical‑safe columns  = 12 */
static int S = 0;                 /* number of states = V*V = 144 */
static int col_bits[VMAX];        /* mapping from local index to 6‑bit column */
static int next_idx[SMAX][SMAX];  /* next state indices for each state */
static int next_cnt[SMAX];        /* number of transitions for each state */
static bool tables_built = false;

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Build tables for states, transitions and the list of vertical
   safe columns.  This is done only once. */
static void build_tables(void)
{
    if (tables_built)
        return;

    /* enumerate vertical‑safe columns */
    int idx = 0;
    for (int bits = 0; bits < 64; ++bits) {
        if (vertical_safe(bits)) {
            col_bits[idx++] = bits;
        }
    }
    V = idx;            /* expected to be 12 */
    S = V * V;          /* expected to be 144 */

    /* transitions between states.  State i = (first, second)
       Transition to state (second, third) if horizontal rule satisfies. */
    for (int first = 0; first < V; ++first) {
        for (int second = 0; second < V; ++second) {
            int state = first * V + second;
            int cnt = 0;
            int bitsA = col_bits[first];
            int bitsB = col_bits[second];
            for (int third = 0; third < V; ++third) {
                int bitsC = col_bits[third];
                bool ok = true;
                for (int r = 0; r < 6; ++r) {
                    int a = (bitsA >> r) & 1;   /* first column at row r */
                    int b = (bitsB >> r) & 1;   /* second column at row r */
                    int c = (bitsC >> r) & 1;   /* third column at row r */
                    if (a == 0 && c == 1) {     /* forbidden 0-?-1 */
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    int next_state = second * V + third;
                    next_idx[state][cnt++] = next_state;
                }
            }
            next_cnt[state] = cnt;
        }
    }

    tables_built = true;
}

/* --------------------------------------------------------------------- */
/* Main sequence function.  Uses the DP described above. */
seq_t a(idx_t n)
{
    build_tables();

    if (n == 1) {
        return (seq_t)V;                 /* only vertical constraint */
    }
    if (n == 2) {
        return (seq_t)V * (seq_t)V;      /* pairs of vertical‑safe columns */
    }

    /* DP arrays for current and next step */
    static seq_t cur[SMAX], nxt[SMAX];
    for (int i = 0; i < S; ++i) {
        cur[i] = 1;                      /* each pair of columns is a start state */
    }

    int steps = (int)(n - 2);             /* number of transitions needed */
    for (int step = 0; step < steps; ++step) {
        for (int i = 0; i < S; ++i)
            nxt[i] = 0;

        for (int state = 0; state < S; ++state) {
            seq_t val = cur[state];
            if (val == 0)
                continue;
            for (int k = 0; k < next_cnt[state]; ++k) {
                int ns = next_idx[state][k];
                nxt[ns] += val;
            }
        }
        for (int i = 0; i < S; ++i)
            cur[i] = nxt[i];
    }

    /* Sum over all ending states to get total */
    seq_t total = 0;
    for (int i = 0; i < S; ++i) {
        total += cur[i];
    }
    return total;
}
