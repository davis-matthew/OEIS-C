/*
 * A207090  Number of 4Xn 0..1 arrays avoiding 0 0 0 horizontally and 0 0 1
 *          vertically.
 * https://oeis.org/A207090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 4×n binary arrays avoiding a horizontal triple
 *            000 and a vertical triple 001. Computed by dynamic programming
 *            over columns with states consisting of the last two columns
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 1) return (seq_t)12;      /* 12 allowed columns */
    if (n == 2) return (seq_t)144;     /* 12² arrangements, no horizontal 000 yet */

    /* allowed 4‑bit columns (no vertical 001) */
    static const uint8_t allowed[12] = {0,1,2,3,5,6,7,10,11,13,14,15};

    /* DP arrays: states are pairs (prev1,prev2), 12×12 = 144 states */
    seq_t dp[144];
    seq_t ndp[144];

    /* initialise all counts to zero */
    for (int i = 0; i < 144; ++i) dp[i] = 0;

    /* base case: all pairs of allowed columns for the first two columns */
    for (int i = 0; i < 12; ++i) {
        for (int j = 0; j < 12; ++j) {
            dp[i*12 + j] = 1;
        }
    }

    /* add columns 3 .. n */
    for (idx_t step = 0; step < n - 2; ++step) {
        for (int i = 0; i < 144; ++i) ndp[i] = 0;

        for (int i = 0; i < 12; ++i) {
            for (int j = 0; j < 12; ++j) {
                seq_t cnt = dp[i*12 + j];
                if (cnt == 0) continue;
                uint8_t prev1 = allowed[i];
                uint8_t prev2 = allowed[j];

                for (int k = 0; k < 12; ++k) {
                    uint8_t newc = allowed[k];
                    bool ok = true;
                    for (int r = 0; r < 4; ++r) {
                        uint8_t b2 = (prev2 >> r) & 1;
                        uint8_t b1 = (prev1 >> r) & 1;
                        uint8_t b0 = (newc >> r) & 1;
                        if (!b2 && !b1 && !b0) { ok = false; break; }
                    }
                    if (!ok) continue;
                    ndp[k*12 + i] += cnt;
                }
            }
        }
        /* swap dp and ndp */
        for (int i = 0; i < 144; ++i) dp[i] = ndp[i];
    }

    /* sum over all states to obtain total number of matrices */
    seq_t total = 0;
    for (int i = 0; i < 144; ++i) total += dp[i];
    return total;
}
