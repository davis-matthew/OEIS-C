/*
 * A135569  a(n) = S2(n)*2^n; where S2(n) is digit sum of n, n in binary
 *          notation.
 * https://oeis.org/A135569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = A000120(n)*2^n
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute S2(n): the binary digit sum (popcount of n). */
    idx_t temp = n;
    seq_t pop = 0;
    while (temp)
    {
        pop += temp & 1;
        temp >>= 1;
    }
    return pow2(n) * pop;
}
