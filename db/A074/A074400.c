/*
 * A074400  Sum of the even divisors of 2n.
 * https://oeis.org/A074400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2 * σ(n) (sum of the even divisors of 2n)
 * Range:     u8 n <= 59, u16 n <= 9239, u32 n <= 9841, u64 n <= 9841,
 *            u128 n <= 9841, big any n
 * Helpers:   _sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_sigma_of.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; index 0 is never used, but we return 0 safely. */
    if (n == 0)
        return (seq_t)0;
    return (seq_t)2 * _sigma_of(n);
}
