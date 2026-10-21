/*
 * A063549  Smallest number of crossing-free matchings on n points in the
 *          plane.
 * https://oeis.org/A063549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Catalan(n/2) if n even else n*Catalan((n-1)/2)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset is 1; value for n=0 is undefined, return 0 */
    idx_t k = n / 2;
    seq_t c = catalan(k);
    if ((n & 1) == 0)   /* even n */
        return c;
    else                /* odd n */
        return (seq_t)n * c;
}
