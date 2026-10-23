/*
 * A020521  13th cyclotomic polynomial evaluated at powers of 2.
 * https://oeis.org/A020521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{12} 2^{k*n}
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 0; k <= 12; ++k)
    {
        res += pow2(k * n);
    }
    return res;
}
