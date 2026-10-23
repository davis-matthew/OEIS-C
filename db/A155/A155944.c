/*
 * A155944  Jacobsthal numbers A001045, every second term incremented by 1.
 * https://oeis.org/A155944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((2^n + 1)/3) + (n mod 2)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t v = pow2(n);               /* 2^n */
    v += 1;                          /* 2^n + 1 */
    v /= 3;                          /* floor division by 3 */
    v += (seq_t)(n & 1ULL);          /* add n mod 2 */
    return v;
}
