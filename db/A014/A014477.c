/*
 * A014477  Expansion of (1 + 2*x)/(1 - 2*x)^3.
 * https://oeis.org/A014477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 * 2^n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t twop = pow2(n);

    /* (n+1)^2 */
    seq_t x = (seq_t)(n + 1);
    x = x * x;

    return twop * x;
}
