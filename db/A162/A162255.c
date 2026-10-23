/*
 * A162255  a(n) = 2*a(n-2) for n > 2; a(1) = 3, a(2) = 2.
 * https://oeis.org/A162255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) with a(1)=3, a(2)=2
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* k = floor(n/2), i.e. n>>1 */
    idx_t k = n >> 1;

    /* compute 2^k */
    seq_t pow2 = pow_int((seq_t)2, k);

    /* even n -> a(n) = 2^k
       odd  n -> a(n) = 3 * 2^k */
    if ((n & 1) == 0)
        return pow2;
    else
        return (seq_t)3 * pow2;
}
