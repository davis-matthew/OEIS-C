/*
 * A233698  Expansion of b(q^2) * c(q^2) / (3 * b(q)^2) in powers of q where
 *          b(), c() are cubic AGM functions.
 * https://oeis.org/A233698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 31, u64 n <= 102, u128 n <= 360,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A233698
 * Formula: Expansion of (η(q²)·η(q³)·η(q⁶) / η(q)³)² in powers of q.
 * Computation: Coefficients of the product
 *   ∏_{m≥1,6∤m} (1 - q^m)^(-g_m)   with  g_m = 6 if m≡1 or 5 (mod 6),
 *                                  g_m = 4 if m≡2,3,4 (mod 6).
 *
 * This function evaluates a(n) for any n ≥ 0 on demand.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Convert to a usable size type */
    size_t N = (size_t)n;
    if (N == 0) return (seq_t)1;  /* a(0) = 1 */

    /* Allocate array for dynamic programming */
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (!dp) return (seq_t)0;      /* allocation failure, unlikely */

    dp[0] = (seq_t)1;              /* base case: 1 way to make 0 */

    /* Dynamic programming over parts m not divisible by 6 */
    for (size_t m = 1; m <= N; ++m)
    {
        if (m % 6 == 0) continue;   /* skip multiples of 6 (g_m = 0) */

        /* g_m depends on residue of m mod 6 */
        unsigned g = (m % 6 == 1 || m % 6 == 5) ? 6 : 4;

        for (unsigned t = 0; t < g; ++t)          /* each “type” of part */
        {
            for (size_t i = m; i <= N; ++i)       /* unbounded coin change */
            {
                dp[i] += dp[i - m];
            }
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
