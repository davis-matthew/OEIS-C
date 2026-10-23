/*
 * A184201  Number of (n+1)X4 binary arrays with no 2X2 subblock containing
 *          fewer than two 1s.
 * https://oeis.org/A184201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A184201: Number of (n+1)×4 binary arrays with no 2×2 subblock containing fewer than two 1s.
   This is computed by dynamic programming on the 16 possible rows.  For each pair of adjacent rows
   the constraint is checked bit‑wise.  Let A be the 16×16 adjacency matrix where
   A[s][t] = 1 iff for all j = 0..2 the 2×2 block with rows s,t and columns j,j+1 contains
   at least two 1s.  Then a(n) = sum_{i,j} (Aⁿ)[i][j], i.e. the total number of paths of
   length n in this graph.  The algorithm iterates n steps, updating a count vector of size 16. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Build adjacency matrix once */
    static bool built = false;
    static unsigned char adj[16][16];
    if (!built) {
        for (int s = 0; s < 16; ++s) {
            for (int t = 0; t < 16; ++t) {
                bool ok = true;
                for (int j = 0; j < 3; ++j) {
                    int sum = ((s >> j) & 1) + ((s >> (j + 1)) & 1)
                           + ((t >> j) & 1) + ((t >> (j + 1)) & 1);
                    if (sum < 2) {
                        ok = false;
                        break;
                    }
                }
                adj[s][t] = ok ? 1 : 0;
            }
        }
        built = true;
    }

    /* dp[s] = number of paths of current length ending in state s */
    seq_t dp[16], next[16];
    for (int s = 0; s < 16; ++s) dp[s] = 1;   /* length 0: start in any state */

    for (idx_t step = 1; step <= n; ++step) {
        for (int t = 0; t < 16; ++t) {
            seq_t sum = 0;
            for (int s = 0; s < 16; ++s) {
                if (adj[s][t]) sum += dp[s];
            }
            next[t] = sum;
        }
        for (int i = 0; i < 16; ++i) dp[i] = next[i];
    }

    seq_t total = 0;
    for (int i = 0; i < 16; ++i) total += dp[i];
    return total;
}
