/*
 * A228581  The number of binary pattern classes in the (2,n)-rectangular grid
 *          with 6 '1's and (2n-6) '0's: two patterns are in same class if one
 *          of them can be obtained by a reflection or 180-degree rotation of
 *          the other.
 * https://oeis.org/A228581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(binomial(2*n,6) + 3*binomial(n,3))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 76, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Binomial term with 2*n choose 6 */
    seq_t bin1 = binomial(n + n, 6);

    /* Binomial term with n choose 3 */
    seq_t bin2 = binomial(n, 3);

    /* Combine as per the formula: (bin1 + 3*bin2) / 4 */
    seq_t result = (bin1 + (bin2 * 3)) / 4;

    return result;
}
