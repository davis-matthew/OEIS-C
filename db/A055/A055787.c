/*
 * A055787  a(n) = 2^(4*n-1) - 2^(2*n-1)*binomial(2*n,n).
 * https://oeis.org/A055787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(4*n-1) - 2^(2*n-1)*binomial(2*n,n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) is defined as 0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute 2^(2*n-1) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < 2*n - 1; ++i)
        pow2 *= (seq_t)2;

    /* Compute 2^(4*n-1) by extending pow2 */
    seq_t pow4 = pow2;
    for (idx_t i = 0; i < 2*n; ++i)
        pow4 *= (seq_t)2;

    /* Compute binomial(2*n, n) */
    seq_t bin = binomial(2*n, n);

    /* Final result: 2^(4*n-1) - 2^(2*n-1)*C(2*n,n) */
    return pow4 - (pow2 * bin);
}
