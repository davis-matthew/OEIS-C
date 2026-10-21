/*
 * A087897  Number of partitions of n into odd parts greater than 1.
 * https://oeis.org/A087897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1‑x^(2k+1))
 * Range:     u8 n <= 45, u16 n <= 107, u32 n <= 294, u64 n <= 908,
 *            u128 n <= 3072, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the number of partitions of n into odd parts ≥ 3           */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build environment */
seq_t a(idx_t n)
{
    /* Allocate DP table for 0 … n */
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp)
        abort();                         /* allocation failure */

    dp[0] = (seq_t)1;                    /* empty partition */

    for (idx_t part = 3; part <= n; part += 2)        /* odd parts ≥3 */
    {
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
