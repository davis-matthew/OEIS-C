/*
 * A202466  Number of (n+2)X5 binary arrays with no more than two of any
 *          consecutive three bits set in any row or column.
 * https://oeis.org/A202466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 13, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A202466:  Number of (n+2)×5 binary arrays with no more than two
 *  of any three consecutive bits set in any row or column.
 *
 *  This implementation uses a standard dynamic‑programming approach:
 *  • All admissible 5‑bit row patterns (no “111”) are enumerated (24 of
 *    them).
 *  • A state is a pair (prevprev,prev) of such patterns.  From a state
 *    we may append a new row pattern curr iff, for each of the five
 *    columns, the triple (prevprev,prev,curr) never contains three 1’s
 *    in that column.
 *  • The DP proceeds column by column, counting the number of ways to
 *    reach each state after each additional row.  The final answer is
 *    the sum of all state counts after the last row.
 */

#include <stdint.h>
#include <stdbool.h>

/* Constants – all are integral constant expressions. */
#define MAX_PATTERNS  32
#define MAX_STATES    576  /* 24 × 24 */

static int  S;                      /* number of admissible row patterns */
static int  stateCount;             /* S × S */
static uint8_t patterns[MAX_PATTERNS];           /* patterns themselves */

/* Transition tables – populated on first call. */
static uint8_t nextCount[MAX_STATES];          /* how many transitions from a state */
static uint8_t nextIndices[MAX_STATES][MAX_PATTERNS]; /* destination indices */

/* Flag to ensure one‑time initialisation. */
static int initDone = 0;

/* --------------------------------------------------------------------- */
/* Initialise the pattern list and the transition tables.                */
/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    if (initDone) return;

    int patCnt = 0;
    /* Enumerate all 5‑bit patterns that avoid the substring 111. */
    for (int p = 0; p < 32; ++p) {
        int ok = 1;
        for (int pos = 0; pos <= 2; ++pos) {
            if (((p >> pos) & 0b111) == 0b111) { ok = 0; break; }
        }
        if (ok) patterns[patCnt++] = (uint8_t)p;
    }
    S = patCnt;                     /* S = 24 */
    stateCount = S * S;             /* 576 */

    /* Build the adjacency lists. */
    for (int i = 0; i < stateCount; ++i) nextCount[i] = 0;
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            int stateIdx = i * S + j;      /* (prevprev,prev) */
            for (int k = 0; k < S; ++k) {
                if ( (patterns[i] & patterns[j] & patterns[k]) == 0 )
                    nextIndices[stateIdx][nextCount[stateIdx]++] = (uint8_t)k;
            }
        }
    }
    initDone = 1;
}

/* --------------------------------------------------------------------- */
/* Public routine:  a(n) – number of admissible matrices.                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_tables();

    /* Number of rows in the matrix = n + 2.  n ≥ 1 ⇒ N ≥ 3. */
    idx_t N = n + 2;

    /* Basic case for N = 3 (n = 1) – handled by DP below.  For completeness
       we keep simple cases for N < 3. */
    if (N == 1) return (seq_t)S;                 /* single row: any pattern */
    if (N == 2) return (seq_t)S * (seq_t)S;       /* first two rows arbitrary */

    /* DP arrays – stored on the stack.  stateCount ≤ 576. */
    seq_t dp[MAX_STATES];
    seq_t ndp[MAX_STATES];

    /* Initialisation: after placing the first two rows, every state is
       reachable exactly once. */
    for (int i = 0; i < stateCount; ++i) dp[i] = 1;

    /* Add rows 3 … N.  The loop runs (N-2) times. */
    for (idx_t step = 2; step < N; ++step) {
        for (int i = 0; i < stateCount; ++i) ndp[i] = 0;

        for (int s = 0; s < stateCount; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;

            uint8_t prev = s % S;   /* prev (second row in the state) */
            uint8_t *dest = nextIndices[s];
            uint8_t  cnt  = nextCount[s];
            for (uint8_t t = 0; t < cnt; ++t) {
                uint8_t curr = dest[t];
                int newState = prev * S + curr;   /* (prev,curr) */
                ndp[newState] += val;
            }
        }
        /* Move ndp into dp for the next iteration. */
        for (int i = 0; i < stateCount; ++i) dp[i] = ndp[i];
    }

    /* Total number of matrices is the sum over all final states. */
    seq_t total = 0;
    for (int i = 0; i < stateCount; ++i) total += dp[i];
    return total;
}
