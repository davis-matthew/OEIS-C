/*
 * A103577  Number of partitions of n into Fibonacci parts if each part is of
 *          two kinds.
 * https://oeis.org/A103577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 121, u64 n <= 718,
 *            u128 n <= 7265, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A103577
 *  Number of partitions of n into Fibonacci parts if each part is of two kinds.
 *  Generating function:
 *          ∏_{i≥2} (1 - x^F_i)^(-2),   with F_2=1, F_3=2, F_4=3, …
 *
 *  Computation:
 *      Dynamic programming treating every Fibonacci value as two independent
 *      coin types.  For each Fibonacci weight f we perform the standard
 *      unlimited‑coin DP twice (once for each color).  The complexity is
 *      O(n log n) and uses O(n) memory.
 *
 *  The function has external linkage and uses only the types seq_t and idx_t
 *  supplied by the build system.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* DP array: dp[t] = number of partitions of t so far */
    seq_t *dp = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!dp)
        return (seq_t)0;          /* out of memory; caller may handle */

    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;

    /* Fibonacci numbers from F_2=1, F_3=2, ... */
    idx_t f_prev = 1;   /* current Fibonacci value */
    idx_t f_curr = 2;   /* next Fibonacci value */

    while (f_prev <= n)
    {
        /* first color of this Fibonacci coin */
        for (idx_t t = f_prev; t <= n; ++t)
            dp[t] += dp[t - f_prev];

        /* second color of this Fibonacci coin */
        for (idx_t t = f_prev; t <= n; ++t)
            dp[t] += dp[t - f_prev];

        /* advance to next Fibonacci number */
        idx_t nxt = f_prev + f_curr;
        f_prev = f_curr;
        f_curr = nxt;
    }

    seq_t result = dp[n];
    free(dp);
    return result;
}
