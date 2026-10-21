/*
 * A207759  Number of nX5 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 1 1 and 1 0 1 vertically.
 * https://oeis.org/A207759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 37, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for OEIS A207759 (Number of n×5 binary arrays with
   horizontal avoidance of 0 0 0 and 1 0 1, and vertical avoidance of
   0 1 1 and 1 0 1) by a transfer‑matrix dynamic programming. */
#include <stdbool.h>

/* Function to be supplied by the build system. */
extern seq_t a(idx_t n);

/* Return the nth term for n≥1, 0 for n==0. */
seq_t a(idx_t n)
{
    /* Special case: for n==0 the sequence is not defined (return 0). */
    if (n == 0)
        return (seq_t)0;

    /* Pre‑compute the allowed 5‑bit patterns and the transition
       table the first time this function is called. */
    static bool initialized = false;
    /* Number of allowed patterns; known to be 13. */
    static int S = 0;
    /* Oldest allowed patterns as integers 0..31. */
    static int patterns[13];
    /* For each pair (i,j) of previous two rows, store the list of
       allowed next rows and the count of them.  13 × 13 = 169 states. */
    static int nextList[169][13];
    static int nextCount[169];

    if (!initialized) {
        /* Find all 5‑bit patterns avoiding 000 and 101 horizontally. */
        int cnt = 0;
        for (int p = 0; p < 32; ++p) {
            bool ok = true;
            for (int j = 0; j <= 2; ++j) {
                int sub = (p >> j) & 7;       /* 3‑bit substring */
                if (sub == 0 || sub == 5) {   /* 000 or 101 */
                    ok = false;
                    break;
                }
            }
            if (ok) {
                patterns[cnt++] = p;
            }
        }
        S = cnt;                          /* should be 13 */

        /* Build transition table from each pair to allowed next rows. */
        for (int i = 0; i < S; ++i) {
            int pi = patterns[i];
            for (int j = 0; j < S; ++j) {
                int pj = patterns[j];
                int pid = i * S + j;        /* pair id */
                int c = 0;
                for (int k = 0; k < S; ++k) {
                    int pk = patterns[k];
                    bool ok = true;
                    for (int col = 0; col < 5 && ok; ++col) {
                        int a = (pi >> col) & 1;
                        int b = (pj >> col) & 1;
                        int c0 = (pk >> col) & 1;
                        /* disallow vertical 0 1 1 or 1 0 1 */
                        if ((a == 0 && b == 1 && c0 == 1) ||
                            (a == 1 && b == 0 && c0 == 1))
                            ok = false;
                    }
                    if (ok) {
                        nextList[pid][c++] = k;
                    }
                }
                nextCount[pid] = c;
            }
        }
        initialized = true;
    }

    /* Special case for n==1: only horizontal constraints, so answer
       is the number of allowed patterns. */
    if (n == 1)
        return (seq_t)S;

    /* Dynamic programming over rows.  dp[pid] holds the number of ways
       to reach state (prevprev_row, prev_row) after processing the
       current number of rows. */
    int numStates = S * S;            /* 169 */
    seq_t dp[169] = {0};
    seq_t nextdp[169] = {0};

    /* Base for n==2: any pair of allowed rows works (no vertical
       triple yet).  Start with all ones. */
    for (int id = 0; id < numStates; ++id)
        dp[id] = (seq_t)1;

    /* Process rows 3 … n.  For each new row we transition from
       (i,j) to (j,k) if allowed vertically. */
    for (idx_t row = 3; row <= n; ++row) {
        /* Zero nextdp */
        for (int id = 0; id < numStates; ++id)
            nextdp[id] = (seq_t)0;

        for (int id = 0; id < numStates; ++id) {
            seq_t val = dp[id];
            if (val == (seq_t)0)
                continue;

            int j = id % S;                 /* current last row index */

            int cnt = nextCount[id];
            for (int t = 0; t < cnt; ++t) {
                int k = nextList[id][t];
                int nid = j * S + k;        /* new pair (prev, next) */
                nextdp[nid] += val;
            }
        }
        /* swap dp and nextdp */
        for (int id = 0; id < numStates; ++id)
            dp[id] = nextdp[id];
    }

    /* Sum all states to obtain the total number of arrays of length n. */
    seq_t ans = (seq_t)0;
    for (int id = 0; id < numStates; ++id)
        ans += dp[id];

    return ans;
}
