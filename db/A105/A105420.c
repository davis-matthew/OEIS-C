/*
 * A105420  Number of partitions of n into 3-smooth parts.
 * https://oeis.org/A105420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts of the form 2^i·3^j
 *            (3‑smooth numbers). The sequence is computed by an unbounded
 *            coin‑change dynamic‑programming using all such numbers ≤ n. This
 *            algorithm is correct for all n ≥ 0
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Allocate a DP table for 0 … n */
    idx_t  size = n + 1;
    seq_t *dp   = (seq_t *)malloc(size * sizeof(seq_t));
    if (dp == NULL)      /* fallback in case of allocation failure */
        return (seq_t)0;

    /* Initialise: dp[0] = 1, others = 0 */
    for (idx_t i = 0; i < size; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Enumerate all 3‑smooth coin values ≤ n and perform the
       classic unbounded‑coins DP update. */
    for (uint64_t pow2 = 1; pow2 <= n; pow2 <<= 1) {
        for (uint64_t pow3 = 1; pow3 <= n / pow2; pow3 *= 3) {
            uint64_t coin = pow2 * pow3;          /* 3‑smooth part */
            for (idx_t s = (idx_t)coin; s <= n; ++s)
                dp[s] += dp[s - (idx_t)coin];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
