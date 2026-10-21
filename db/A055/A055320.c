/*
 * A055320  Number of labeled trees with n nodes and 8 leaves.
 * https://oeis.org/A055320
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n!/8!)*Stirling2(n-2,n-8)
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 12, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*
 * Computes a(n) for n >= 9.
 * Uses an iterative DP for the Stirling numbers of the second kind
 * with fixed difference d = 6 (since n-2 - (n-8) = 6).
 * The factorial part n!/8! is replaced by the product of 9..n.
 */
seq_t a(idx_t n)
{
    if (n < 9) return 0;                // undefined for n < 9

    /* factorial part: n! / 8! = product of integers from 9 to n */
    seq_t fact_fac = 1;
    for (idx_t i = 9; i <= n; ++i) {
        fact_fac *= (seq_t)i;
    }

    /* DP arrays for Stirling numbers S(t, t-d) with d = 0..6 */
    enum { D = 6, SZ = D + 1 };
    seq_t prev[SZ] = {0}, curr[SZ] = {0};

    /* base: S(0,0) = 1 */
    prev[0] = 1;

    idx_t m = n - 2;   /* we need S(m, m-6) */
    for (idx_t t = 1; t <= m; ++t) {
        curr[0] = 1;              /* S(t,t) = 1 */

        for (int d = 1; d <= D; ++d) {
            seq_t val = 0;
            if (t > (idx_t)d) {   /* (t-d) * S(t-1, t-d) term */
                val += (seq_t)(t - d) * prev[d - 1];
            }
            val += prev[d];        /* + S(t-1, t-d-1) term */
            curr[d] = val;
        }

        /* copy curr -> prev for next iteration */
        for (int d = 0; d <= D; ++d) prev[d] = curr[d];
    }

    /* result: n!/8! * S(n-2, n-8)  == fact_fac * prev[6] */
    return fact_fac * prev[6];
}
