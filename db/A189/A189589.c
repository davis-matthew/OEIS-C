/*
 * A189589  Number of permutations of 1..n with displacements restricted to
 *          {-5,-4,-2,-1,0,3}.
 * https://oeis.org/A189589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 110,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count permutations of 1..n with displacements in {-5,-4,-2,-1,0,3} */
/* Uses a sliding‑window DP of width 9 – one bit per possible value in each window */

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    static const int DISPL[6] = { -5, -4, -2, -1, 0, 3 };
    seq_t dp[512];
    seq_t next[512];

    /* initialise dp to zero */
    for (int k = 0; k < 512; ++k) dp[k] = 0;

    /* base mask for i = 1 : values i-5 ... i+3 that are already out of range are marked 1 */
    unsigned baseMask = 0;
    for (int idx = 0; idx < 9; ++idx) {
        int val = idx - 4;                 /* val = 1 + (idx-5) when i = 1 */
        if (val < 1 || val > (int)n) baseMask |= (1U << idx);
    }
    dp[baseMask] = 1;

    seq_t answer = 0;

    for (idx_t i = 1; i <= n; ++i) {
        /* zero next buffer */
        for (int k = 0; k < 512; ++k) next[k] = 0;

        /* required bits for this position: every value in the current window that lies in 1..n must be used by the end */
        unsigned requiredMask = 0;
        for (int idx = 0; idx < 9; ++idx) {
            int val = (int)i - 5 + idx;
            if (val >= 1 && val <= (int)n)
                requiredMask |= (1U << idx);
        }

        for (unsigned mask = 0; mask < 512; ++mask) {
            seq_t ways = dp[mask];
            if (!ways) continue;

            for (int dIdx = 0; dIdx < 6; ++dIdx) {
                int d   = DISPL[dIdx];
                int val = (int)i + d;
                if (val < 1 || val > (int)n) continue;

                unsigned idx = d + 5;          /* bit position corresponding to this displacement */
                if (mask & (1U << idx)) continue;  /* value already used */

                unsigned usedMask = mask | (1U << idx);

                if (i == n) {   /* last position – check that all required bits are used */
                    if ((usedMask & requiredMask) == requiredMask)
                        answer += ways;
                } else {
                    unsigned newBit = (i + 4 > n) ? 1U : 0U;    /* value i+4 will be out of range? */
                    unsigned newMask = (usedMask >> 1) | (newBit << 8);
                    next[newMask] += ways;
                }
            }
        }
        /* prepare for next iteration */
        for (int k = 0; k < 512; ++k) dp[k] = next[k];
    }
    return answer;
}
