/*
 * A018925  Number of isotropic subspaces of a 2n-dimensional unitary space
 *          over GF(4).
 * https://oeis.org/A018925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Product_{k=1..n} (4^k + 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0; /* undefined for offset 1 */

    seq_t prod = 1;
    for (idx_t k = 1; k <= n; ++k)
    {
        seq_t factor = pow2(2 * k); /* 4^k = 2^(2k) */
        factor += 1;                /* factor = 4^k + 1 */
        prod *= factor;             /* running product */
    }
    return prod;
}
