/*
 * A072856  Number of permutations satisfying i-4<=p(i)<=i+4, i=1..n
 *          (permutations of length n within distance 4).
 * https://oeis.org/A072856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of permutations of {1,…,n} such that i‑4 ≤ p(i) ≤
 *            i+4 for all i. Computed via a sliding window DP with a 9‑bit
 *            state representing which of the columns i‑4 … i+4 are still
 *            free. O(n·512) time, O(512) memory
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1; /* empty permutation count */

    /* initial state for i = 1 */
    int start_mask = 0;
    for (int k = 0; k < 9; ++k) {
        int col = 1 - 4 + k;                     /* j = i-4 + k, i=1 */
        if (col < 1 || col > 5 || col > (int)n)  /* not a valid free column */
            start_mask |= (1 << k);              /* mark as used */
    }

    seq_t dp[512]      = {0};
    seq_t next_dp[512] = {0};
    dp[start_mask] = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i) {
        /* clear next state */
        for (int m = 0; m < 512; ++m) next_dp[m] = 0;

        for (int mask = 0; mask < 512; ++mask) {
            seq_t ways = dp[mask];
            if (ways == 0) continue;

            /* free columns are bits that are 0 */
            int freebits = (~mask) & 0x1FF;     /* 9‑bit mask */

            while (freebits) {
                int k = __builtin_ctz(freebits);   /* choose a free column */
                freebits &= freebits - 1;          /* clear that bit */

                int mask_used = mask | (1 << k);          /* mark chosen column used */
                int shifted   = (mask_used >> 1);          /* shift for next row */

                /* new column j = i+5 : free if it exists within [1,n] */
                if (i + 5 <= n)
                    shifted &= ~(1 << 8);  /* free -> bit 8 = 0 */
                else
                    shifted |= (1 << 8);   /* out of range -> used */

                next_dp[shifted] += ways;
            }
        }

        /* swap dp and next_dp */
        for (int m = 0; m < 512; ++m) dp[m] = next_dp[m];
    }

    /* after processing all rows, all columns <= n must be used:
       bits 0,1,2,3 correspond to columns n-3 … n, all must be 1. */
    seq_t result = 0;
    for (int mask = 0; mask < 512; ++mask) {
        if ((mask & 0xF) == 0xF)  /* bits 0‑3 are all 1 */
            result += dp[mask];
    }
    return result;
}
