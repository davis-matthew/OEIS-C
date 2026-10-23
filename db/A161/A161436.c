/*
 * A161436  Number of reduced words of length n in the Weyl group A_4.
 * https://oeis.org/A161436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of reduced words of length n in the Weyl group A_4
   (the Weyl group of type A_4 is the symmetric group S5).
   This equals the number of integer solutions to a1 + a2 + a3 + a4 = n
   with 0 ≤ a_k ≤ k.  The generating function is ∏_{k=1..4}(1+x+…+x^k),
   which is non‑zero only for n = 0…10.  The sequence is:
   1,4,9,15,20,22,20,15,9,4,1,0,0,…   (length 11).  Imple­ments memoized DP. */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* The maximum length in S5 is binomial(5,2)=10.  No reduced words
       of longer length exist. */
    if (n > 10)
        return (seq_t)0;

    static seq_t dp[11];
    static bool init = false;

    if (!init) {
        /* Initialize to 0 and set dp[0] = 1. */
        for (idx_t i = 0; i <= 10; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* For each k = 1..4, convolve with (1 + x + … + x^k). */
        for (idx_t k = 1; k <= 4; ++k) {
            seq_t newdp[11] = {0};
            for (idx_t i = 0; i <= 10; ++i) {
                if (dp[i] == 0)
                    continue;
                for (idx_t t = 0; t <= k && i + t <= 10; ++t) {
                    newdp[i + t] += dp[i];
                }
            }
            for (idx_t i = 0; i <= 10; ++i)
                dp[i] = newdp[i];
        }
        init = true;
    }
    return dp[n];
}
