/*
 * A116377  Number of partitions of n into parts with digital root = 7.
 * https://oeis.org/A116377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of partitions of n into parts with digital root 7
 *            * (i.e., integer parts that are congruent to 7 modulo 9)
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h> /* malloc, free */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;          /* offset starts at 1 */
    /* Allocate an array of seq_t for dynamic programming */
#ifdef __cplusplus
    seq_t *dp = new seq_t[n + 1]();       /* value‑initialized (zero) */
#else
    seq_t *dp = malloc((n + 1) * sizeof(seq_t));
    if (!dp) return 0;                    /* allocation failed */
    for (idx_t i = 0; i <= n; ++i) dp[i] = 0; /* zero‑initialize */
#endif

    dp[0] = 1; /* empty partition */

    /* For every part that is ≡7 (mod 9) add its contributions */
    for (idx_t p = 7; p <= n; p += 9) {
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }

    seq_t result = dp[n];
#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
