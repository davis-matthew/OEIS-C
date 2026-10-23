/*
 * A156232  a(n) is the number of induced subgraphs with odd number of edges
 *          in the cycle graph C(n).
 * https://oeis.org/A156232
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(n-1) - 2^(n/2) if n is even; a(n) = 2^(n-1) if n is
 *            odd (OEIS A156232)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2) /* behaviour below offset is unspecified */
        return 0;

    seq_t pow_n1 = pow2(n - 1);
    if ((n & 1) == 0) /* even n */
    {
        seq_t pow_half = pow2(n / 2);
        return pow_n1 - pow_half;
    }
    return pow_n1; /* odd n */
}
