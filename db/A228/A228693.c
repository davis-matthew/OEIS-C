/*
 * A228693  Largest number of maximal independent sets of nodes in any tree on
 *          n nodes.
 * https://oeis.org/A228693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 * = 2^((n-1)/2) if n is odd * = 2^(n/2-1)+1 if
 *            n is even ≥2 * (equivalently a(n) = 3*a(n-2) – 2*a(n-4) for n >
 *            4)
 * Range:     u8 n <= 16, u16 n <= 32, u32 n <= 64, u64 n <= 128,
 *            u128 n <= 256, big any n
 * Helpers:   pow2_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2_binary.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;                       /* base case */

    if (n & 1)                          /* odd n */
    {
        /* exponent = (n-1)/2 */
        idx_t exp = (n - 1) >> 1;
        return pow2_binary(exp);
    }
    else                                 /* even n ≥ 2 */
    {
        idx_t exp = (n >> 1) - 1;        /* n/2 - 1 */
        seq_t val = pow2_binary(exp);
        return val + 1;
    }
}
