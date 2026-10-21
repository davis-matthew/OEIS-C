/*
 * A239078  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 6: |p(i)-i| <= 6.
 * https://oeis.org/A239078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A239078:  Number of self‑inverse permutations p on [n]
   with displacement |p(i)-i| <= 6.  The permutation consists of
   fixed points and 2‑cycles; the condition on displacement means
   that a 2‑cycle (i,j) must satisfy |i-j| <= 6.
   This is equivalent to counting the number of matchings in the graph
   whose vertices are 1..n and whose edges are all pairs (i,j) with
   1 <= i < j <= n and j-i <= 6.  The following dynamic programme
   counts those matchings. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* DP over the vertices.  The state is a 6‑bit mask describing
       which of the last six vertices (relative to the next vertex to
       be processed) are currently unmatched and therefore may be
       matched with future vertices.  The mask bit i (0‑based) corresponds
       to the vertex at distance i+1 from the current position. */
    seq_t dp[64] = {0}, dp2[64];
    dp[0] = 1;                      /* before any vertex, no unmatched ones */

    for (idx_t v = 1; v <= n; ++v) {
        /* reset dp2 */
        for (int m = 0; m < 64; ++m) dp2[m] = 0;

        for (int m = 0; m < 64; ++m) {
            seq_t val = dp[m];
            if (val == 0) continue;

            /* Case 1: leave vertex v unmatched.  It becomes a new
               unmatched vertex at distance 1. */
            int nm = ((m << 1) & 0x3F) | 1;
            dp2[nm] += val;

            /* Case 2: match vertex v with one of the previous
               unmatched vertices that are within distance 6.  For each
               such vertex, remove it from the mask and shift. */
            for (int d = 1; d <= 6; ++d) {
                if (m & (1 << (d - 1))) {
                    int temp = m & ~(1 << (d - 1));
                    int nm2 = ((temp << 1) & 0x3F);
                    dp2[nm2] += val;
                }
            }
        }
        /* copy dp2 to dp for the next iteration */
        for (int m = 0; m < 64; ++m) dp[m] = dp2[m];
    }

    /* After processing all vertices, any remaining unmatched
       vertices (encoded in the mask) are simply fixed points,
       so we sum over all masks. */
    seq_t total = 0;
    for (int m = 0; m < 64; ++m) total += dp[m];
    return total;
}
