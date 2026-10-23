/*
 * A034146  Number of partitions of n into distinct parts from [1, 16].
 * https://oeis.org/A034146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=1}^{16} (1 + x^k) (partitions
 *            of n into distinct parts from 1..16)
 * Range:     u8 n <= 32, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Number of partitions of n into distinct parts from 1..16. */
seq_t a(idx_t n)
{
    /* Convert n to size_t for array indexing; the caller guarantees n is
       small enough for the required computation (≈10,000). */
    size_t N = (size_t)n;

    /* Allocate array of size N+1.  The array is freed before returning. */
    seq_t *dp = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (dp == NULL)
        return 0;      /* allocation failure: return 0 as a safe fallback */

    /* Initialize all entries to 0, with dp[0] = 1 (empty partition). */
    for (size_t i = 0; i <= N; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Standard subset‑sum DP over the parts 1..16. */
    for (int k = 1; k <= 16; ++k) {
        for (size_t s = N + 1; s-- > (size_t)k; ) {   /* s runs from N down to k */
            dp[s] += dp[s - k];
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
