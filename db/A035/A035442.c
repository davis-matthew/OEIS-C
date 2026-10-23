/*
 * A035442  Number of partitions of n into parts 8k or 8k+2.
 * https://oeis.org/A035442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into parts 8k or 8k+2
 * Range:     u8 n <= 63, u16 n <= 167, u32 n <= 497, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Sequence is defined for n >= 1; for n < 1 return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Allocate an array of size n+1 and initialise to 0. */
    size_t sz = (size_t)n + 1;
    seq_t *dp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* allocation failure – unlikely in tests */

    dp[0] = (seq_t)1;             /* empty partition */

    /* Iterate over all allowed part sizes (even numbers ≡ 0 or 2 mod 8). */
    for (idx_t p = 2; p <= n; p += 2) {
        if ((p & 7) == 0 || (p & 7) == 2) {   /* p == 8k or 8k+2 */
            for (idx_t i = p; i <= n; ++i) {
                dp[i] += dp[i - p];
            }
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
