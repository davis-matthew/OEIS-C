/*
 * A022215  Gaussian binomial coefficients [ n,8 ] for q = 5.
 * https://oeis.org/A022215
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{i=1..8} (5^(n-i+1)-1)/(5^i-1) (n ≥ 8)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 9, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the recurrence: G(n,k) = G(n-1,k) + 5^(n-k)·G(n-1,k-1), with k=8 */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 8)
        return 0;          /* undefined for indices below the offset */

    seq_t prev[9] = {0};
    seq_t curr[9] = {0};

    prev[0] = 1;           /* G(0,0) = 1, G(0,k>0) = 0 */

    for (idx_t i = 1; i <= n; ++i) {
        curr[0] = 1;       /* G(i,0) = 1 */

        for (idx_t k = 1; k <= 8; ++k) {
            if (k > i) {
                curr[k] = 0;           /* G(i,k) = 0 when k > i */
                continue;
            }

            seq_t power = pow_uint(5, i - k);      /* 5^(i-k) */
            curr[k] = prev[k] + power * prev[k - 1];
        }

        /* copy current row into previous for next iteration */
        for (idx_t k = 0; k <= 8; ++k)
            prev[k] = curr[k];
    }

    return prev[8];        /* a(n) = G(n,8) */
}
