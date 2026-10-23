/*
 * A116376  Number of partitions of n into parts with digital root = 6.
 * https://oeis.org/A116376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into parts with digital rest 6
 *            (i.e., parts % 9 == 6)
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n < 1 we return 0. */
    if (n < 1) return 0;

    /* Allocate a DP table: dp[i] will hold the number of partitions of i. */
    size_t len = (size_t)n + 1;
    seq_t *dp = (seq_t*)malloc(len * sizeof(seq_t));
    if (!dp) return 0;               /* out‑of‑memory guard, unlikely */

    /* Initialise to zero; seq_t may be a builtin integer or a big‑int class,
       zero is representable by the literal 0. */
    for (size_t i = 0; i < len; ++i) dp[i] = 0;
    dp[0] = 1;                       /* empty partition */

    /* Unrestricted coin change DP using parts that are 6 mod 9. */
    for (idx_t part = 6; part <= n; part += 9) {
        for (idx_t i = part; i <= n; ++i) {
            dp[i] += dp[i - part];
        }
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
