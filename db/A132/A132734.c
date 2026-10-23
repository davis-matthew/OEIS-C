/*
 * A132734  Row sums of triangle A132733.
 * https://oeis.org/A132734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; a(n) = 4*2^n - 5*n - 1 for n > 0
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t pow2n = pow2(n);
    seq_t res   = (seq_t)4 * pow2n;          /* 4*2^n       */
    res = res - (seq_t)5 * (seq_t)n - 1;     /* -5*n - 1    */
    return res;
}
