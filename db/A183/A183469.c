/*
 * A183469  Number of nX4 binary arrays with every element equal to either the
 *          sum mod 2 of its vertical neighbors or the sum mod 2 of its
 *          horizontal neighbors.
 * https://oeis.org/A183469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A183469 – number of n×4 binary arrays whose each entry is equal
 *            to the sum modulo 2 of either its vertical neighbours or of its
 *            horizontal neighbours. Computation method: - Each row is a 4‑bit
 *            pattern (0…15). - For any three consecutive rows (p,c,n) all
 *            four cells must satisfy c[j] == (p[j] + n[j...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Constants for the state space */
enum {
    ROWS   = 16,          /* 4‑bit rows: 0 … 15 */
    STATES = ROWS * ROWS, /* (prev,row) pairs */
};

/* Transition lists: for each state, list of reachable states */
static int  nextDest[STATES][ROWS];
static int  nextCount[STATES];

/* Whether a state can be followed by a dummy zero row */
static bool finalMask[STATES];

/*----------------------------------------------------------------------------*/
/* Test whether a triple (prev, cur, next) satisfies all cell constraints. */
static bool triple_valid(int p, int c, int n)
{
    for (int j = 0; j < 4; ++j) {
        int pv = (p >> j) & 1;
        int nv = (n >> j) & 1;
        int cv = (c >> j) & 1;
        int vertical   = pv ^ nv;                 /* sum mod 2 of vertical neighbours */
        int left  = (j > 0)  ? ((c >> (j - 1)) & 1) : 0;
        int right = (j < 3)  ? ((c >> (j + 1)) & 1) : 0;
        int horizontal = left ^ right;            /* sum mod 2 of horizontal neighbours */
        if (cv != vertical && cv != horizontal)
            return false;
    }
    return true;
}

/* Build transition tables once. */
static void build_transition(void)
{
    for (int p = 0; p < ROWS; ++p) {
        for (int c = 0; c < ROWS; ++c) {
            int s = p * ROWS + c;
            nextCount[s] = 0;
            finalMask[s] = triple_valid(p, c, 0);
            for (int n = 0; n < ROWS; ++n) {
                if (triple_valid(p, c, n)) {
                    int dest = c * ROWS + n;          /* state after adding row n */
                    nextDest[s][nextCount[s]++] = dest;
                }
            }
        }
    }
}

/* Public entry point – returns a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    /* Ensure the transition graph is built only once. */
    static bool initialized = false;
    if (!initialized) {
        build_transition();
        initialized = true;
    }

    if (n < 1)
        return (seq_t)0;

    /* DP arrays over states. */
    seq_t dp[STATES];
    seq_t next[STATES];

    /* initialise: first row, previous row is all zeros. */
    for (int i = 0; i < STATES; ++i)
        dp[i] = 0;
    for (int r = 0; r < ROWS; ++r)
        dp[r] = 1;               /* state (0, r) has one way */

    /* Process additional rows (from the 2nd to the n‑th). */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i)
            next[i] = 0;
        for (int s = 0; s < STATES; ++s) {
            if (dp[s] == 0) continue;
            seq_t val = dp[s];
            int cnt = nextCount[s];
            for (int k = 0; k < cnt; ++k) {
                int dest = nextDest[s][k];
                next[dest] += val;
            }
        }
        for (int i = 0; i < STATES; ++i)
            dp[i] = next[i];
    }

    /* Sum over states that can transition to a zero row. */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s)
        if (finalMask[s])
            result += dp[s];

    return result;
}
