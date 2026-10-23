/*
 * A169992  Expansion of Product_{i=0..m-1} (1 + x^(2*i+1)) for m=9.
 * https://oeis.org/A169992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into distinct odd parts ≤ 17
   (i.e. coefficients of ∏_{i=0}^{8} (1 + x^{2i+1}) ).
   Computed by subset–sum DP over the set {1,3,5,7,9,11,13,15,17}. */
#include <stdbool.h>

seq_t a(idx_t n) {
    /* maximum sum with all parts: 1+3+5+7+9+11+13+15+17 = 81 */
    static seq_t dp[82];
    static bool ready = false;

    if (!ready) {
        /* initialize dp array */
        for (idx_t i = 0; i < 82; ++i) dp[i] = 0;
        dp[0] = 1;

        /* list of odd exponents */
        const idx_t odds[9] = {1,3,5,7,9,11,13,15,17};

        for (int k = 0; k < 9; ++k) {
            idx_t e = odds[k];
            for (idx_t j = 81; j >= e; --j) {
                dp[j] = dp[j] + dp[j - e];
            }
        }
        ready = true;
    }

    if (n > 81) return 0;
    return dp[n];
}
