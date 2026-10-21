/*
 * A236782  Number of (n+1)X(1+1) 0..2 arrays with the upper median plus the
 *          lower median minus the minimum of every 2X2 subblock equal.
 * https://oeis.org/A236782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 15, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: counting (n+1)×2 arrays over {0,1,2} whose
   every 2×2 subblock satisfies
       upper-median + lower-median – minimum  ≡  constant
   (the constant may be any value 0…4).  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* All possible 2‑tuples of {0,1,2} – 9 rows in total. */
    static const int row[9][2] = {
        {0,0},{0,1},{0,2},
        {1,0},{1,1},{1,2},
        {2,0},{2,1},{2,2}
    };

    /* Pre‑compute, for each constant value c=0..4,
       whether a transition from row s to row t is valid. */
    static bool adj[5][9][9];
    static bool inited = false;
    if (!inited) {
        for (int c = 0; c <= 4; ++c) {
            for (int s = 0; s < 9; ++s) {
                int a0 = row[s][0], a1 = row[s][1];
                for (int t = 0; t < 9; ++t) {
                    int b0 = row[t][0], b1 = row[t][1];
                    int v[4] = {a0, a1, b0, b1};
                    /* sort v[0..3] */
                    for (int i = 0; i < 3; ++i)
                        for (int j = i + 1; j < 4; ++j)
                            if (v[i] > v[j]) {
                                int tmp = v[i];
                                v[i] = v[j];
                                v[j] = tmp;
                            }
                    int val = v[2] + v[1] - v[0];
                    adj[c][s][t] = (val == c);
                }
            }
        }
        inited = true;
    }

    seq_t answer = 0;
    seq_t dp[9], next[9];

    /* Enumerate all possible constants c. */
    for (int c = 0; c <= 4; ++c) {
        /* First row can be any of the 9 rows. */
        for (int i = 0; i < 9; ++i) dp[i] = (seq_t)1;

        /* Perform n transitions to build n+1 rows. */
        for (idx_t step = 1; step <= n; ++step) {
            for (int i = 0; i < 9; ++i) next[i] = 0;
            for (int s = 0; s < 9; ++s) {
                if (dp[s] == 0) continue;
                seq_t cnt = dp[s];
                for (int t = 0; t < 9; ++t)
                    if (adj[c][s][t])
                        next[t] += cnt;
            }
            for (int i = 0; i < 9; ++i) dp[i] = next[i];
        }

        /* Sum all ending rows for this constant c. */
        for (int i = 0; i < 9; ++i) answer += dp[i];
    }

    return answer;
}
