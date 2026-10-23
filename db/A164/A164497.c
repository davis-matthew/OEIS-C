/*
 * A164497  Number of binary strings of length n with no substrings equal to
 *          0010 0110 or 1011.
 * https://oeis.org/A164497
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A164497: number of binary strings of length n that do not contain
   the forbidden substrings 0010, 0110, or 1011.
   The algorithm uses a 3‑bit state DP: at each step we keep the
   last up to three bits of the current string and extend by 0 or 1.
   For steps i ≥ 4 we reject any transition that would produce a
   forbidden 4‑bit pattern.  The result is the sum of all states after n steps.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t i;
    const int forbid[3] = {2, 6, 11};   /* 0010, 0110, 1011 */
    seq_t dp[8] = {0}, ndp[8];

    dp[0] = 1;   /* empty string */

    for (i = 1; i <= n; ++i) {
        ndp[0] = ndp[1] = ndp[2] = ndp[3] =
        ndp[4] = ndp[5] = ndp[6] = ndp[7] = 0;

        for (int s = 0; s < 8; ++s) {
            seq_t val = dp[s];
            if (!val) continue;

            for (int bit = 0; bit < 2; ++bit) {
                int ns = ((s << 1) | bit) & 7;
                if (i >= 4) {
                    int pat = ((s << 1) | bit);
                    if (pat == forbid[0] || pat == forbid[1] || pat == forbid[2])
                        continue;
                }
                ndp[ns] += val;
            }
        }

        /* copy back to dp */
        for (int s = 0; s < 8; ++s)
            dp[s] = ndp[s];
    }

    seq_t sum = 0;
    for (int s = 0; s < 8; ++s)
        sum += dp[s];

    return sum;
}
