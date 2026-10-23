/*
 * A161816  Denominator of fraction in Redheffer type matrix.
 * https://oeis.org/A161816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(1) = 1, a(2) = 2, a(3) = 4, a(n) = 2^(n-2) for n >= 4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)2;
    if (n == 3)
        return (seq_t)4;
    return pow2(n - 2);
}
