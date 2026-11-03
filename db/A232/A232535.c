/*
 * A232535  Triangle T(n,k), 0 <= k <= n, read by rows defined by: T(n,k) =
 *          (binomial(2*n,2*k) + binomial(2*n+1,2*k))/2.
 * https://oeis.org/A232535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (binomial(2*m,2*r) + binomial(2*m+1,2*r)) / 2, where m =
 *            floor((sqrt(8*n+1)-1)/2) and r = n - m*(m+1)/2. This reproduces
 *            OEIS A232535
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 57 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find m such that m*(m+1)/2 <= n < (m+1)*(m+2)/2. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    /* r = n - T_m, where T_m = m*(m+1)/2. It satisfies 0 <= r <= m. */
    idx_t r = n - m * (m + 1) / 2;

    /* Compute T(m,r) = (binomial(2m,2r) + binomial(2m+1,2r)) / 2. */
    seq_t t1 = binomial(2 * m, 2 * r);
    seq_t t2 = binomial(2 * m + 1, 2 * r);

    return (t1 + t2) / 2;
}
