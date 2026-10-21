/*
 * A202643  Number of (n+2)X6 binary arrays avoiding patterns 000 and 111 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   row_has_run_of_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dynamic programming over rows using a two‑row state automaton.
 * The state consists of the last two rows (each 6 bits).  Transitions
 * impose the restrictions that the new row has no run of 3 equal bits
 * in a row, and that for each column and each NW‑to‑SE diagonal the
 * last three bits are never all equal.  The DP counts all valid
 * (n+2)×6 binary arrays for a given n.  The first term of the
 * sequence (n = 1) corresponds to 3 rows.  The algorithm works for
 * any n ≥ 1 and runs in O(n·states·transitions).  The maximum number
 * of states is 64² = 4096.  The transition table is built once on
 * the first call. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/row_has_run_of_three.h"

/* ------------------------------------------------------------------ */
/* Forward declaration of the function required by the test harness. */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Static data built on first call ---------------------------------------- */
#define MAX_PAT 64
#define MAX_STATE 4096         /* 64×64  */

static bool    built = false;
static int     P;                 /* number of admissible row patterns  */
static int     S;                 /* number of states P×P              */

static unsigned char pat_bits[MAX_PAT][6]; /* bit table for each pattern */
static int           row_pat[MAX_PAT];     /* list of admissible rows      */

static int   next_count[MAX_STATE];           /* transitions for each src    */
static int   next_dst[MAX_STATE][MAX_PAT];    /* destination indices         */

static seq_t dp_cur[MAX_STATE];
static seq_t dp_next[MAX_STATE];

/* ------------------------------------------------------------------ */
static void build_tables(void)
{
    /* ---- compile list of acceptable row patterns and bit tables */
    int pat_idx = 0;
    for (int pat = 0; pat < MAX_PAT; ++pat) {
        /* store bits once */
        for (int c = 0; c < 6; ++c)
            pat_bits[pat][c] = (pat >> c) & 1;
        if (row_has_run_of_three(pat)) {
            row_pat[pat_idx++] = pat;
        }
    }
    P = pat_idx;
    S = P * P;

    /* ---- build transition table */
    for (int i = 0; i < P; ++i) {
        int pat2 = row_pat[i];
        for (int j = 0; j < P; ++j) {
            int pat1 = row_pat[j];
            int src  = i * P + j;
            int cnt  = 0;
            for (int k = 0; k < P; ++k) {
                int cur = row_pat[k];
                /* ---- column triple constraint ---- */
                bool bad = false;
                for (int c = 0; c < 6 && !bad; ++c) {
                    if (pat_bits[pat2][c] == pat_bits[pat1][c] &&
                        pat_bits[pat1][c] == pat_bits[cur][c]) {
                        bad = true;
                    }
                }
                if (bad) continue;

                /* ---- NW‑to‑SE diagonal triple constraint ---- */
                for (int c = 1; c <= 4 && !bad; ++c) {
                    if (pat_bits[pat2][c - 1] == pat_bits[pat1][c] &&
                        pat_bits[pat1][c]      == pat_bits[cur][c + 1]) {
                        bad = true;
                    }
                }
                if (bad) continue;

                /* valid transition */
                next_dst[src][cnt++] = j * P + k;   /* new state: (prev1, cur) */
            }
            next_count[src] = cnt;
        }
    }
    built = true;
}

/* ------------------------------------------------------------------ */
/* Main function ------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!built) build_tables();

    /* Number of additional rows after the first two */
    idx_t steps = n;

    /* initialise DP for two rows (no column or diagonal constraints) */
    for (int i = 0; i < S; ++i)
        dp_cur[i] = 0;
    for (int i = 0; i < P; ++i)
        for (int j = 0; j < P; ++j)
            dp_cur[i * P + j] = 1;

    /* iterate additional rows */
    for (idx_t step = 0; step < steps; ++step) {
        /* clear next dp */
        for (int i = 0; i < S; ++i)
            dp_next[i] = 0;
        /* transition */
        for (int src = 0; src < S; ++src) {
            seq_t val = dp_cur[src];
            if (!val)
                continue;
            int cnt = next_count[src];
            for (int d = 0; d < cnt; ++d) {
                int dst = next_dst[src][d];
                dp_next[dst] += val;
            }
        }
        /* swap dp_cur and dp_next */
        for (int i = 0; i < S; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* total sum of all states = required number of arrays */
    seq_t total = 0;
    for (int i = 0; i < S; ++i)
        total += dp_cur[i];

    return total;
}
