/*
 * A234030  Number of (n+1)X(2+1) 0..4 arrays with every 2X2 subblock having
 *          the absolute values of all six edge and diagonal differences no
 *          larger than 1.
 * https://oeis.org/A234030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of (n+1)×3 arrays with entries 0..4 for which
 *            every * 2×2 subblock has all six edge and diagonal differences
 *            no larger than 1
 * Range:     u8 none, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Pre‑compute the set of horizontal states and the transition matrix.
       These are done only once, on the first call. */
    static bool initialized = false;
    static uint8_t state[125][3];          /* all valid horizontal triples   */
    static uint8_t trans_mat[125][125];    /* 1 if two states can follow each other */
    static int ns = 0;                    /* number of states               */

    if (!initialized) {
        /* Generate all triples (a,b,c) with |a-b|<=1 and |b-c|<=1.     */
        for (int a0 = 0; a0 <= 4; ++a0)
            for (int b0 = 0; b0 <= 4; ++b0)
                for (int c0 = 0; c0 <= 4; ++c0)
                    if (abs(a0 - b0) <= 1 && abs(b0 - c0) <= 1) {
                        state[ns][0] = (uint8_t)a0;
                        state[ns][1] = (uint8_t)b0;
                        state[ns][2] = (uint8_t)c0;
                        ++ns;
                    }

        /* Build the transition matrix.  A transition from i to j is
           allowed when all vertical and diagonal constraints are met.  */
        for (int i = 0; i < ns; ++i) {
            for (int j = 0; j < ns; ++j) {
                int ok = 1;
                /* vertical edges */
                if (abs((int)state[i][0] - (int)state[j][0]) > 1) ok = 0;
                if (abs((int)state[i][1] - (int)state[j][1]) > 1) ok = 0;
                if (abs((int)state[i][2] - (int)state[j][2]) > 1) ok = 0;
                /* diagonals */
                if (abs((int)state[i][0] - (int)state[j][1]) > 1) ok = 0;
                if (abs((int)state[i][1] - (int)state[j][0]) > 1) ok = 0;
                if (abs((int)state[i][1] - (int)state[j][2]) > 1) ok = 0;
                if (abs((int)state[i][2] - (int)state[j][1]) > 1) ok = 0;
                trans_mat[i][j] = (uint8_t)ok;
            }
        }
        initialized = true;
    }

    /* Dynamic programming over the rows.  dp[i] holds the number of ways
       to reach state i after the current number of rows.             */
    static seq_t dp[125];
    static seq_t dp_next[125];

    /* First row: any valid state is possible. */
    for (int i = 0; i < ns; ++i)
        dp[i] = 1;

    /* There are (n+1) rows in total; we already have the first one.
       So we perform n transitions. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < ns; ++i)
            dp_next[i] = (seq_t)0;
        for (int i = 0; i < ns; ++i) {
            seq_t cur = dp[i];
            if (cur == 0) continue;
            for (int j = 0; j < ns; ++j)
                if (trans_mat[i][j])
                    dp_next[j] += cur;
        }
        for (int i = 0; i < ns; ++i)
            dp[i] = dp_next[i];
    }

    /* Sum over all final states gives the total number of valid arrays. */
    seq_t total = 0;
    for (int i = 0; i < ns; ++i)
        total += dp[i];

    return total;
}
