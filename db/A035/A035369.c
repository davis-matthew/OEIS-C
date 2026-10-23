/*
 * A035369  Number of partitions of n into parts 5k or 5k+3.
 * https://oeis.org/A035369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 5k or 5k+3
 * Range:     u8 n <= 52, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is signed 64‑bit; we only need 0 ≤ n ≤ UINT64_MAX */
    if (n == 0)
        return (seq_t)0;  /* not used because offset is 1 */

    /* Allocate a dynamic programming array of size n+1. */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)  /* allocation failed – return 0 (not a valid sequence value) */
        return (seq_t)0;

    /* Initialise: dp[i] = number of partitions of i with allowed parts. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;          /* empty partition of 0 */

    /* Dynamic‑programming: iterate over allowed part sizes. */
    for (idx_t part = 3; part <= n; ++part) {
        if (part % 5 == 0 || part % 5 == 3) {
            for (idx_t sum = part; sum <= n; ++sum) {
                dp[sum] += dp[sum - part];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
