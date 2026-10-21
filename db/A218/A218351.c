/*
 * A218351  Number of n X 5 binary arrays with every 0 a horizontal or
 *          vertical neighbor to some 1.
 * https://oeis.org/A218351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Uses dynamic programming over the rows of an n×5 binary matrix.
 * The state is (previous row pattern, mask of zeros in that row
 * that do not have a horizontal 1 neighbour and are therefore
 * required to have a 1 below).  Transitions check that all such
 * zeros are satisfied by a 1 in the current row and compute the
 * new required mask for the current row.  The answer is the sum
 * of states after n rows with no remaining unsatisfied zeros.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define WIDTH 5
#define PAT (1 << WIDTH)           /* 32 */
#define MASK (PAT - 1)             /* 0x1F */
#define MAX_STATE (PAT * PAT)      /* 1024 */

/* Mapping from (pattern, unsatisfied mask) to state index */
static int state_id[PAT][PAT];
/* Reverse mapping for each state */
static int state_pat[MAX_STATE];
static int state_unsat[MAX_STATE];
/* Transition table: state_id[p][u] -- next pattern pn --> next state */
static int trans[MAX_STATE][PAT];
/* Horizontal unsatisfied mask for each pattern */
static int horiz_unsat[PAT];
static int state_count;
static bool precomputed = false;

/* Precompute tables once */
static void precompute(void)
{
    int p, u, s, pn;
    int new_unsat;

    /* initialise state ids to -1 */
    for (p = 0; p < PAT; ++p)
        for (u = 0; u < PAT; ++u)
            state_id[p][u] = -1;

    /* compute horizontal unsatisfied mask for every pattern */
    for (p = 0; p < PAT; ++p) {
        int mask = 0;
        for (int j = 0; j < WIDTH; ++j) {
            if (!((p >> j) & 1)) {
                bool left  = (j > 0)     && ((p >> (j - 1)) & 1);
                bool right = (j < WIDTH-1) && ((p >> (j + 1)) & 1);
                if (!left && !right)
                    mask |= 1 << j;
            }
        }
        horiz_unsat[p] = mask;
    }

    /* enumerate valid states */
    s = 0;
    for (p = 0; p < PAT; ++p) {
        int valid = horiz_unsat[p];
        for (u = 0; u < PAT; ++u) {
            if ((u & ~valid) == 0) {          /* u ⊆ valid */
                state_id[p][u] = s;
                state_pat[s] = p;
                state_unsat[s] = u;
                ++s;
            }
        }
    }
    state_count = s;

    /* build transition table */
    for (s = 0; s < state_count; ++s) {
        int prev_p = state_pat[s];
        int prev_u = state_unsat[s];
        for (pn = 0; pn < PAT; ++pn) {
            /* previous unsatisfied zeros must be satisfied by a 1 in current row */
            if ((prev_u & ~pn) != 0) {
                trans[s][pn] = -1;
                continue;
            }
            /* compute required mask for current row */
            new_unsat = (~pn) & horiz_unsat[pn] & (~prev_p);
            new_unsat &= MASK;                /* keep only width bits */
            trans[s][pn] = state_id[pn][new_unsat]; /* guaranteed to exist */
        }
    }

    precomputed = true;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (!precomputed)
        precompute();

    if (n == 0)                     /* offset starts at 1; value for 0 is unused */
        return (seq_t)0;

    static seq_t dp_cur[MAX_STATE];
    static seq_t dp_next[MAX_STATE];
    int s, pn;
    unsigned i;

    /* clear current DP */
    for (i = 0; i < (unsigned)state_count; ++i)
        dp_cur[i] = 0;

    /* initial states: first row */
    for (int p = 0; p < PAT; ++p) {
        s = state_id[p][horiz_unsat[p]];
        dp_cur[s] = (seq_t)1;
    }

    /* process remaining rows (if any) */
    for (idx_t r = 1; r < n; ++r) {
        for (i = 0; i < (unsigned)state_count; ++i)
            dp_next[i] = 0;
        for (s = 0; s < state_count; ++s) {
            seq_t val = dp_cur[s];
            if (!val)
                continue;
            for (pn = 0; pn < PAT; ++pn) {
                int ns = trans[s][pn];
                if (ns >= 0)
                    dp_next[ns] += val;
            }
        }
        /* swap dp arrays */
        for (i = 0; i < (unsigned)state_count; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum over states with no remaining unsatisfied zeros */
    seq_t total = 0;
    for (s = 0; s < state_count; ++s) {
        if (state_unsat[s] == 0)
            total += dp_cur[s];
    }
    return total;
}
