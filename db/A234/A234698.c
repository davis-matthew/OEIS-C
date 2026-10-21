/*
 * A234698  Number of (n+1) X (2+1) 0..7 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 5
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A234698
   Computed by transfer-matrix dynamic programming.
   Let S be the set of all 3‑digit tuples over {0,…,7} (512 states).
   For r,s∈S, r → s is allowed iff for each of the two 2×2
   subblocks formed by r and s the difference of the diagonal sum
   and the anti‑diagonal sum equals 5 in absolute value:
        |(r[j] + s[j+1]) – (r[j+1] + s[j])| = 5   for j = 0,1.
   Then a(n) equals the number of walks of length n in this graph
   (arrays of height n+1).  The algorithm runs in O(n) time with
   512 states and a few thousand transitions.
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ──────────────────────  Build transition map  ────────────────────── */
    static int    outdeg[512];
    static int    next_tbl[512][512];
    static int    adjacency_built = 0;

    if (!adjacency_built) {
        for (int r = 0; r < 512; ++r) {
            int a = (r >> 6) & 7;
            int b = (r >> 3) & 7;
            int c = r & 7;
            outdeg[r] = 0;

            for (int s = 0; s < 512; ++s) {
                int d = (s >> 6) & 7;
                int e = (s >> 3) & 7;
                int f = s & 7;

                int diff1 = (a + e) - (b + d);
                if (diff1 < 0) diff1 = -diff1;
                if (diff1 != 5) continue;

                int diff2 = (b + f) - (c + e);
                if (diff2 < 0) diff2 = -diff2;
                if (diff2 != 5) continue;

                next_tbl[r][outdeg[r]++] = s;
            }
        }
        adjacency_built = 1;
    }

    /* ──────────────────────  Dynamic programming  ────────────────────── */
    seq_t ways[512];
    seq_t newways[512];

    for (idx_t i = 0; i < 512; ++i)
        ways[i] = 1;                 /* one array for each possible first row */

    for (idx_t step = 1; step <= n; ++step) {
        for (int i = 0; i < 512; ++i)
            newways[i] = 0;

        for (idx_t i = 0; i < 512; ++i) {
            seq_t w = ways[i];
            if (w == 0) continue;
            int deg = outdeg[i];
            for (int k = 0; k < deg; ++k) {
                int j = next_tbl[i][k];
                newways[j] += w;
            }
        }

        for (int i = 0; i < 512; ++i)
            ways[i] = newways[i];
    }

    seq_t total = 0;
    for (idx_t i = 0; i < 512; ++i)
        total += ways[i];

    return total;
}
