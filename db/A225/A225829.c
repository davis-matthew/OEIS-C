/*
 * A225829  Number of binary pattern classes in the (5,n)-rectangular grid:
 *          two patterns are in the same class if one of them can be obtained
 *          by a reflection or 180-degree rotation of the other.
 * https://oeis.org/A225829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(5n/2-1)*(2^(5n/2-1)+2^(n/2-1)+1) if n even, a(n) =
 *            2^((5n-1)/2-1)*(2^((5n-1)/2)+2^((n-1)/2)+5) if n odd
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 25,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    if (n & 1)   /* odd n */
    {
        idx_t m = (5 * n - 1) / 2;
        seq_t term1 = pow2(m - 1);
        seq_t inner = pow2(m) + pow2((n - 1) / 2) + 5;
        return term1 * inner;
    }
    else   /* even n */
    {
        idx_t m = (5 * n) / 2;
        seq_t term1 = pow2(m - 1);
        seq_t inner = pow2(m - 1) + pow2(n / 2 - 1) + 1;
        return term1 * inner;
    }
}
