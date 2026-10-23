/*
 * A164904  a(n) is the number of palindromic structures using a maximum of
 *          ten different symbols.
 * https://oeis.org/A164904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1}^{min(10, ceil(n/2))} S(ceil(n/2), k) where
 *            S(m,k) is the Stirling number of the second kind
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the Stirling numbers incrementally up to k = 10
   and sums them for the floor/ceil of n/2. */
#include <stdint.h>
#include <stdbool.h>

/* Helper for multiplying a seq_t by a small unsigned integer.
   The helper is available in the build; if not, this can be
   replaced with plain multiplication for integer types. */
#include "../utils/mul_small.h"

seq_t a(idx_t n) {
    /* a(0) = 1 */
    if (n == 0) {
        return 1;
    }

    /* m = ceil(n/2) = (n + 1) / 2 for unsigned integers */
    idx_t m = (n + 1) / 2;

    /* dp[k] will hold S(current_length, k) for k = 0..10 */
    seq_t dp[11];
    seq_t newdp[11];
    for (int i = 0; i <= 10; ++i) dp[i] = 0;
    dp[0] = 1;   /* S(0,0) = 1 */

    for (idx_t len = 1; len <= m; ++len) {
        newdp[0] = 0;
        for (int k = 1; k <= 10; ++k) {
            seq_t term1 = mul_small(dp[k], (unsigned)k); /* k * dp[k] */
            seq_t term2 = dp[k - 1];
            newdp[k] = term1 + term2;
        }
        /* Copy newdp back into dp for next iteration */
        for (int k = 0; k <= 10; ++k) dp[k] = newdp[k];
    }

    /* Sum the Stirling numbers for k = 1 .. min(10, m) */
    int maxk = (m < 10) ? (int)m : 10;
    seq_t ans = 0;
    for (int k = 1; k <= maxk; ++k) {
        ans += dp[k];
    }

    return ans;
}
