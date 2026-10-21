/*
 * A052551  Expansion of 1/((1 - x)*(1 - 2*x^2)).
 * https://oeis.org/A052551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^(floor(n/2)+1) - 1
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 63, u64 n <= 127,
 *            u128 n <= 255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t k = n >> 1;               /* floor(n/2) */
    seq_t v = pow2(k + 1);          /* 2^(k+1)       */
    return v - (seq_t)1;            /* subtract 1    */
}
