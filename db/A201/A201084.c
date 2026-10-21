/*
 * A201084  Number of -4..4 arrays of n elements with first and second
 *          differences also in -4..4.
 * https://oeis.org/A201084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count sequences of length n with values in [-4,4] such that
   both first differences and second differences belong to [-4,4].
   The state of the dynamic programming is (current value, next difference).
   For n = 1 the answer is 9 (any value). For n ≥ 2, the count is obtained
   by iterating over all valid states and applying the transition rules. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define MIN_VAL -4
#define MAX_VAL  4
#define VAL_RANGE (MAX_VAL - MIN_VAL + 1)   /* 9 */
#define MAX_STATES (VAL_RANGE * VAL_RANGE)  /* 81 */

static int state_x[MAX_STATES];
static int state_d[MAX_STATES];
static int idx_map[VAL_RANGE][VAL_RANGE];
static int neigh[MAX_STATES][VAL_RANGE];
static int deg[MAX_STATES];
static int nstates = 0;
static bool states_inited = false;

static void init_states(void)
{
    if (states_inited) return;

    /* initialise mapping to -1 */
    for (int i = 0; i < VAL_RANGE; ++i)
        for (int j = 0; j < VAL_RANGE; ++j)
            idx_map[i][j] = -1;

    /* enumerate all valid states (x,d) with x in [-4,4], d in [-4,4],
       and x+d in [-4,4] */
    int s = 0;
    for (int i = MIN_VAL; i <= MAX_VAL; ++i) {
        for (int j = MIN_VAL; j <= MAX_VAL; ++j) {
            int a = i + j;
            if (a >= MIN_VAL && a <= MAX_VAL) {
                idx_map[i - MIN_VAL][j - MIN_VAL] = s;
                state_x[s] = i;
                state_d[s] = j;
                ++s;
            }
        }
    }
    nstates = s;

    /* build adjacency lists */
    for (int i = 0; i < nstates; ++i) {
        int x = state_x[i];
        int d = state_d[i];
        int out = 0;
        for (int j = MIN_VAL; j <= MAX_VAL; ++j) {
            int d_next = j;
            /* second‑difference constraint */
            if (d_next - d < MIN_VAL || d_next - d > MAX_VAL) continue;
            int x_next = x + d;         /* next array element */
            int idx = idx_map[x_next - MIN_VAL][d_next - MIN_VAL];
            if (idx >= 0) {
                neigh[i][out++] = idx;
            }
        }
        deg[i] = out;
    }

    states_inited = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for offset 1   */
    if (n == 1) return (seq_t)9;      /* a(1) = 9                 */

    init_states();

    /* dp_cur and dp_next store counts for each state after a given number
       of transitions (i.e., after the current length).                */
    seq_t dp_cur[MAX_STATES];
    seq_t dp_next[MAX_STATES];

    /* initialise counts for the first transition (length 2) */
    for (int i = 0; i < nstates; ++i)
        dp_cur[i] = (seq_t)1;

    /* number of further transitions */
    idx_t steps = n - 2;   /* when n=2, steps=0 (no further transition) */

    for (idx_t step = 0; step < steps; ++step) {
        /* reset next counts */
        for (int i = 0; i < nstates; ++i)
            dp_next[i] = (seq_t)0;

        /* propagate counts along edges */
        for (int i = 0; i < nstates; ++i) {
            seq_t val = dp_cur[i];
            if (val == (seq_t)0) continue;
            int k;                 /* local to the inner loop */
            for (k = 0; k < deg[i]; ++k) {
                int j = neigh[i][k];
                dp_next[j] += val;
            }
        }
        /* move to next step */
        for (int i = 0; i < nstates; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all states to get total number of arrays of length n */
    seq_t sum = (seq_t)0;
    for (int i = 0; i < nstates; ++i)
        sum += dp_cur[i];

    return sum;
}
