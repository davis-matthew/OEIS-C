/*
 * A209409  Number of subsets of {1,...,n} containing {a,a+2,a+4} for some a.
 * https://oeis.org/A209409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n – number of binary strings of length n that avoid
 *            having ones at positions i, i+2, and i+4 simultaneously (i ≥ 1)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* DP over the last four bits of the prefix.  */
    seq_t dp[16];
    seq_t next[16];

    /* initialize dp for empty string */
    for (int i = 0; i < 16; ++i) dp[i] = 0;
    dp[0] = 1;      /* one empty string */

    for (idx_t pos = 0; pos < n; ++pos) {
        for (int i = 0; i < 16; ++i) next[i] = 0;

        for (int st = 0; st < 16; ++st) {
            seq_t cnt = dp[st];
            if (cnt == 0) continue;

            /* add a 0 bit */
            int new_st = (st << 1) & 0xE;   /* shift left, drop msb, LSB=0 */
            next[new_st] += cnt;

            /* add a 1 bit – check forbidden pattern */
            if (!((st & 0x8) && (st & 0x2))) {  /* st bit3 & bit1 are 1? */
                new_st = ((st << 1) & 0xE) | 1; /* LSB=1 */
                next[new_st] += cnt;
            }
        }

        /* copy next to dp for next iteration */
        for (int i = 0; i < 16; ++i) dp[i] = next[i];
    }

    /* total number of avoiding strings */
    seq_t avoid = 0;
    for (int i = 0; i < 16; ++i) avoid += dp[i];

    /* result = 2^n - avoid */
    return pow2(n) - avoid;
}
