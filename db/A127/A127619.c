/*
 * A127619  Number of walks from (0,0) to (n,n) in the region 0 <= x-y <= 5
 *          with the steps (1,0), (0, 1), (2,0) and (0,2).
 * https://oeis.org/A127619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A127619: number of walks from (0,0) to (n,n) using steps (1,0),(0,1),(2,0),(0,2)
   staying in the band 0 ≤ x–y ≤ 5.  The implementation counts all sequences of
   ±1 and ±2 steps whose absolute sum equals 2n and whose partial sums stay in
   [0,5].  This is equivalent to the required walks.  The algorithm runs in
   O(n) time and uses O(n) memory. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cumulative absolute sum must be exactly 2*n */
    idx_t maxs = 2 * n;
    size_t stride = 6;  /* states for difference 0..5 */

    /* allocate DP table: (maxs+1) layers, each with 6 entries */
    seq_t *dp = (seq_t *)malloc((maxs + 1) * stride * sizeof(seq_t));
    if (!dp) return 0;  /* out of memory – caller should handle */

    /* initialise all entries to 0 */
    for (size_t i = 0; i < (maxs + 1) * stride; ++i)
        dp[i] = 0;

    /* start from difference 0 with sum 0 */
    dp[0 * stride + 0] = 1;

    /* dynamic programming over increasing absolute sum */
    for (idx_t s = 0; s < maxs; ++s) {
        size_t base  = (size_t)s       * stride;
        size_t base1 = (size_t)(s + 1) * stride;   /* transitions of size 1 */
        size_t base2 = (size_t)(s + 2) * stride;   /* transitions of size 2 */

        for (int d = 0; d < 6; ++d) {
            seq_t val = dp[base + d];
            if (!val) continue;

            /* step +1 */
            if (d + 1 <= 5) dp[base1 + d + 1] += val;
            /* step -1 */
            if (d >= 1)     dp[base1 + d - 1] += val;

            /* step +2 (only if we don't exceed maxs) */
            if (s + 2 <= maxs) {
                if (d + 2 <= 5) dp[base2 + d + 2] += val;
                if (d >= 2)     dp[base2 + d - 2] += val;
            }
        }
    }

    seq_t result = dp[maxs * stride + 0];
    free(dp);
    return result;
}
