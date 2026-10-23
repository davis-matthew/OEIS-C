/*
 * A103424  Expansion of e.g.f.: 1 + sinh(2*x).
 * https://oeis.org/A103424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^n - (-2)^n)/2 (a(0)=1, a(n) is 0 for even n>0, 2^n for
 *            odd n)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 by convention (0^0 contributes 1) */
    if (n == 0)
        return (seq_t)1;

    /* Even n > 0 gives 0 */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* Odd n: a(n) = 2^n */
    return pow2(n);
}
