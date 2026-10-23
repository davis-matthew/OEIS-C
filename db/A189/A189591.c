/*
 * A189591  Number of permutations of 1..n with displacements restricted to
 *          {-5,-4,-1,0,2,3}.
 * https://oeis.org/A189591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of 1..n whose displacements belong
 *            to the set {-5, -4, -1, 0, 2, 3}. The implementation uses a
 *            sliding‑window dynamic programming approach that works for all n
 *            ≥ 1 and for any seq_t/idx_t types supplied by the build
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Allowed displacements and sliding window parameters. */
    static const int displacements[6] = { -5, -4, -1, 0, 2, 3 };
    static const int minOffset     = -5;       /* window starts at i-5   */
    static const int maxOffset     =  3;       /* window ends   at i+3   */
    static const int width         = maxOffset - minOffset + 1; /* 9 */
    static const int totalMasks    = 1 << width;               /* 512 */

    /* DP arrays: dp holds counts for the current position,
       next for the next position.  They are zero‑initialized on each call. */
    seq_t dp[totalMasks];
    seq_t next[totalMasks];
    for (int m = 0; m < totalMasks; ++m) {
        dp[m] = 0;
        next[m] = 0;
    }
    dp[0] = 1;   /* no values used at the start */

    /* Main DP loop over positions 1..n. */
    for (idx_t i = 1; i <= n; ++i) {
        /* clear next array */
        for (int m = 0; m < totalMasks; ++m) next[m] = 0;

        /* For each mask state at position i */
        for (int m = 0; m < totalMasks; ++m) {
            seq_t ways = dp[m];
            if (!ways) continue;   /* skip empty states */

            /* Try each allowed displacement */
            for (int k = 0; k < 6; ++k) {
                int d     = displacements[k];
                int bit   = d - minOffset;    /* 0 .. width-1 */
                if (m & (1 << bit)) continue; /* this value already used */

                /* Value that would be placed at position i */
                int64_t val = (int64_t)i + (int64_t)d;
                if (val < 1 || val > (int64_t)n) continue; /* out of range */

                /* Set this value as used and shift mask for next position. */
                int newMask = (m | (1 << bit)) >> 1;
                next[newMask] += ways;
            }
        }

        /* Move to next position */
        for (int m = 0; m < totalMasks; ++m) dp[m] = next[m];
    }

    /* Sum all final mask states to obtain the total number of permutations. */
    seq_t result = 0;
    for (int m = 0; m < totalMasks; ++m) result += dp[m];
    return result;
}
