/*
 * A029143  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^6)). Molien series
 *          for u.g.g.r. #31 of order 46080. Poincaré series [or Poincare
 *          series] for ring of even weight Siegel modular forms of genus 2.
 * https://oeis.org/A029143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 5, and 6
 * Range:     u8 n <= 57, u16 n <= 405, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Parts allowed in the partitions */
    static const idx_t parts[4] = { 2, 3, 5, 6 };
    const size_t   num_parts   = 4;

    /* Allocate an array to hold the DP values */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* out‑of‑memory: return 0 as a safe fallback */

    dp[0] = 1;                    /* base case: one way to make sum 0 */

    /* Classic coin‑change dynamic programming:
       for each part in non‑decreasing order, add its contribution
       to all achievable sums. */
    for (size_t p_idx = 0; p_idx < num_parts; ++p_idx) {
        idx_t p = parts[p_idx];
        if (p > n)
            continue;
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
