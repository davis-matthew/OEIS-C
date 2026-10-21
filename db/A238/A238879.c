/*
 * A238879  Row sums of the triangle of generalized ballot numbers A238762.
 * https://oeis.org/A238879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(2n) = Catalan(n+1), a(2n+1) = (n+1)/2 * Catalan(n+2)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n & 1)
    {
        idx_t k = n >> 1;                /* (n-1)/2 */
        seq_t cat = catalan(k + 2);
        seq_t mult = cat * (seq_t)(k + 1);
        return mult / 2;
    }
    else
    {
        idx_t k = n >> 1;                /* n/2 */
        return catalan(k + 1);
    }
}
