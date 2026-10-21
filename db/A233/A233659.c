/*
 * A233659  Number of (n+1)X(1+1) 0..6 arrays with every 2X2 subblock having
 *          the sum of the squares of the edge differences equal to 26.
 * https://oeis.org/A233659
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of (n+1)×2 arrays with entries 0…6 such that
 *            every 2×2 subblock has the sum of the squares of the
 *            edge‑differences equal to 26. Computed by dynamic programming
 *            over a 49–state graph whose states are the 49 possible rows
 *            (a,b). A transition from (a,b) to (c,d) is allowed iff (a−...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define STATES 49

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* undefined for n < 1 */

    /* adjacency matrix for the 49×49 graph */
    static unsigned char adj[STATES][STATES];
    static int initialized = 0;

    if (!initialized) {
        for (int s = 0; s < STATES; ++s) {
            int a = s / 7;
            int b = s % 7;
            for (int t = 0; t < STATES; ++t) {
                int c = t / 7;
                int d = t % 7;
                int sum = (a - b) * (a - b)
                        + (c - d) * (c - d)
                        + (a - c) * (a - c)
                        + (b - d) * (b - d);
                adj[s][t] = (sum == 26);
            }
        }
        initialized = 1;
    }

    /* DP arrays: cur[i] = number of ways to reach state i after the
       current number of rows.  Initially, after 0 rows, each state is
       reachable in one way (the empty array). */
    seq_t cur[STATES];
    seq_t nxt[STATES];
    for (int i = 0; i < STATES; ++i)
        cur[i] = 1;

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < STATES; ++i)
            nxt[i] = 0;
        for (int u = 0; u < STATES; ++u) {
            if (cur[u] == 0) continue;
            seq_t val = cur[u];
            for (int v = 0; v < STATES; ++v)
                if (adj[u][v])
                    nxt[v] += val;
        }
        for (int i = 0; i < STATES; ++i)
            cur[i] = nxt[i];
    }

    /* sum over all states to count all arrays of length n+1 rows */
    seq_t total = 0;
    for (int i = 0; i < STATES; ++i)
        total += cur[i];

    return total;
}
