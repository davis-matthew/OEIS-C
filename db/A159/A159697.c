/*
 * A159697  a(0) = 9, a(n) = 2*a(n-1) + 2^(n-1) for n > 0.
 * https://oeis.org/A159697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (18 + n) * 2^(n-1) (n ≥ 1, a(0) = 9)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)9;
    return ((seq_t)n + 18) * pow2(n - 1);
}
