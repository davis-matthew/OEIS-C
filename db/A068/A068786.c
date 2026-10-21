/*
 * A068786  S(n; 1,0) = S(n; 3,0) where S(n; t,s) is the number of length n
 *          4-ary strings whose digits sum to t mod 4 and whose sum of
 *          products of all pairs of digits sum to s mod 4.
 * https://oeis.org/A068786
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: S(n; t, s) = S(n‑1; t, s) + S(n‑1; t+3, s+3t+1) +
 *                      S(n‑1; t+2, s+2t) + S(n‑1; t+1, s+t+1) (mod 4).  */
/* The required values are a(n) = S(n; 1, 0).  Indexing starts at n = 1. */

#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"
#include <stddef.h>   /* for size_t */

static struct {
    idx_t   max_n;          /* largest n for which dp is current   */
    seq_t  *values;         /* memoised a(k) for k ≤ max_n          */
    size_t  cap;            /* allocated size of values array       */
    seq_t   dp[4][4];       /* current DP state for length = max_n  */
} state = {0, NULL, 0, {{0}}};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset (outside OEIS) */

    /* Initialise if first call */
    if (state.max_n == 0) {
        /* allocate at least space for index 0 and 1 */
        state.values = seq_alloc( (size_t)2 );
        state.cap    = 2;
        state.values[0] = (seq_t)0;        /* unused, harmless */

        /* base case n = 0 : only the empty string */
        for (int t = 0; t < 4; ++t)
            for (int s = 0; s < 4; ++s)
                state.dp[t][s] = (seq_t)0;
        state.dp[0][0] = (seq_t)1;        /* S(0;0,0) = 1 */

        state.max_n = 0;
    }

    /* Extend DP so that we have computed up to n */
    if (n > state.max_n) {
        /* enlarge cache if needed */
        if ((size_t)n + 1 > state.cap) {
            state.values = seq_realloc(state.values, state.cap, (size_t)n + 1);
            state.cap = (size_t)n + 1;
        }

        /* iterate from current max_n + 1 up to requested n */
        for (idx_t step = state.max_n + 1; step <= n; ++step) {
            seq_t next[4][4];
            for (int t = 0; t < 4; ++t)
                for (int s = 0; s < 4; ++s)
                    next[t][s] = (seq_t)0;

            for (int t = 0; t < 4; ++t) {
                for (int s = 0; s < 4; ++s) {
                    seq_t cur = state.dp[t][s];
                    if (cur == (seq_t)0) continue;
                    for (int d = 0; d < 4; ++d) {
                        int tnew = (t + d) & 3;                 /* (t+d) mod 4 */
                        int snew = (s + d * t) & 3;             /* (s + d*t) mod 4 */
                        next[tnew][snew] += cur;
                    }
                }
            }

            /* memoise a(step) = S(step; 1, 0) */
            state.values[step] = next[1][0];

            /* copy next into current dp for next iteration */
            for (int t = 0; t < 4; ++t)
                for (int s = 0; s < 4; ++s)
                    state.dp[t][s] = next[t][s];
        }

        state.max_n = n;
    }

    return state.values[n];
}
