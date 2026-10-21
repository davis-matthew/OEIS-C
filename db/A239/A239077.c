/*
 * A239077  Number of self-inverse permutations p on [n] with displacement of
 *          elements restricted by 5: |p(i)-i| <= 5.
 * https://oeis.org/A239077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 47, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counting self‑inverse permutations p on [n] with |p(i)-i| <= 5
   The sequence equals the number of matchings in the graph where
   vertices 1..n are connected iff their distance is at most 5.
   An involution is a collection of disjoint 1‑cycles (fixed points)
   and 2‑cycles (transpositions).  The dynamic program keeps a
   5‑bit mask describing which of the last five processed vertices
   are still unmatched and could be paired with later vertices. */

seq_t a(idx_t n)
{
    /* 5‑bit mask keeps track of the last five vertices that are
       still unmatched before the next vertex is processed. */
    const unsigned int MASK5 = 0x1F;   /* (1<<5)-1 */
    seq_t dp[32] = {0};    /* dp[mask] - number of ways for processed prefix */
    seq_t ndp[32];
    dp[0] = 1;             /* no vertices processed yet */

    for (idx_t pos = 1; pos <= n; ++pos) {
        /* reset next array */
        for (int m = 0; m < 32; ++m) ndp[m] = 0;

        for (int mask = 0; mask < 32; ++mask) {
            seq_t val = dp[mask];
            if (val == 0) continue;

            /* Case 1: vertex 'pos' is a fixed point (remains unmatched) */
            unsigned int newMask = ((mask << 1) | 1) & MASK5;
            ndp[newMask] += val;

            /* Case 2: vertex 'pos' is matched with one of the
               previous unmatched vertices within distance 5. */
            for (int j = 0; j < 5; ++j) {
                if (mask & (1u << j)) {        /* vertex pos-1-j is available */
                    newMask = ((mask & ~(1u << j)) << 1) & MASK5;
                    ndp[newMask] += val;
                }
            }
        }

        /* copy back for next position */
        for (int m = 0; m < 32; ++m) dp[m] = ndp[m];
    }

    /* After processing all vertices, any remaining unmatched vertices
       are fixed points.  Sum over all possible masks. */
    seq_t result = 0;
    for (int m = 0; m < 32; ++m) result += dp[m];
    return result;
}
