/*
 * A207492  Number of 7Xn 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 1 1 and 1 1 0 vertically.
 * https://oeis.org/A207492
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 17, u64 n <= 40, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function counts the number of 7×n binary arrays that avoid the patterns
 * 000 and 011 horizontally (three consecutive columns in a row) and
 * 011 and 110 vertically (three consecutive rows in a column).  The dynamic
 * programming enumerates all admissible 7‑bit column patterns and transitions
 * between consecutive columns that respect the horizontal constraints. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define MAX_COLS   128            /* maximum number of possible 7‑bit columns */
#define MAX_STATE  (MAX_COLS * MAX_COLS) /* maximum number of column‑pairs */

static int          initialized = 0;   /* flag whether we have pre‑computed all */
static uint8_t      valid[MAX_COLS];   /* admissible 7‑bit columns */
static int          V;                /* number of admissible columns */

static int *next_count = NULL;        /* number of outgoing transitions for each state */
static int *next_states_flat = NULL;  /* flattened list of transitions, size = statecnt*V */
static const int    MAX_TRANS = MAX_COLS; /* maximum possible transitions from a state */

static seq_t dp_cur_arr[MAX_STATE];
static seq_t dp_next_arr[MAX_STATE];

/* ----------------------------------------------------------------------- */
static void initialise(void)
{
    int vcnt = 0;
    /* ---- admissible vertical columns ---------------------------------- */
    for (int v = 0; v < (1 << 7); v++) {
        int ok = 1;
        for (int r = 0; r <= 7 - 3 && ok; r++) {
            int tri = (v >> r) & 7;
            if (tri == 0b011 || tri == 0b110)
                ok = 0;
        }
        if (ok) {
            valid[vcnt++] = (uint8_t)v;
        }
    }
    V = vcnt;                                 /* V <= 35 for 7 rows */

    /* ---- adjacency between column pairs ------------------------------- */
    int statecnt = V * V;
    next_count      = (int *)malloc(statecnt * sizeof(int));
    next_states_flat = (int *)malloc(statecnt * MAX_TRANS * sizeof(int));
    if (!next_count || !next_states_flat)
        abort(); /* Out of memory */

    for (int a = 0; a < V; a++) {
        for (int b = 0; b < V; b++) {
            int s        = a * V + b;
            int base     = s * MAX_TRANS;
            int cnt = 0;
            for (int c = 0; c < V; c++) {
                int ok = 1;
                for (int r = 0; r < 7 && ok; r++) {
                    int bit_a = (valid[a] >> r) & 1;
                    int bit_b = (valid[b] >> r) & 1;
                    int bit_c = (valid[c] >> r) & 1;
                    int trip  = (bit_a << 2) | (bit_b << 1) | bit_c;
                    if (trip == 0b000 || trip == 0b011)
                        ok = 0;
                }
                if (ok) {
                    next_states_flat[base + cnt] = b * V + c;
                    cnt++;
                }
            }
            next_count[s] = cnt;
        }
    }
    initialized = 1;
}
/* ----------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!initialized)
        initialise();

    /* base cases -------------------------------------------------------- */
    if (n == 1) return (seq_t)V;
    if (n == 2) return (seq_t)V * (seq_t)V;

    /* DP for n >= 3 ----------------------------------------------------- */
    int statecnt = V * V;
    seq_t *cur    = dp_cur_arr;
    seq_t *next   = dp_next_arr;

    /* initial state: all column pairs are allowed for length 2 */
    for (int i = 0; i < statecnt; i++)
        cur[i] = 1;

    int steps = (int)(n - 2); /* number of transitions to reach length n */
    for (int step = 0; step < steps; step++) {
        /* clear next array */
        memset(next, 0, statecnt * sizeof(seq_t));

        /* transition */
        for (int s = 0; s < statecnt; s++) {
            seq_t val = cur[s];
            if (!val)
                continue;
            int base = s * MAX_TRANS;
            int cnt  = next_count[s];
            for (int k = 0; k < cnt; k++) {
                int t = next_states_flat[base + k];
                next[t] += val;
            }
        }

        /* swap pointers for next iteration */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum all end states ----------------------------------------------- */
    seq_t total = 0;
    for (int i = 0; i < statecnt; i++)
        total += cur[i];

    return total;
}
