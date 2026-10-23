/*
 * A174317  a(0)=1, a(1)=2, a(2)=1; for n>2, a(n) = 7*2^(n-3)-2.
 * https://oeis.org/A174317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=1; a(n)=7*2^(n-3)-2 for n>2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 1;
    seq_t val = pow2(n - 3);   /* compute 2^(n-3) */
    val = val * 7;
    val = val - 2;
    return val;
}
