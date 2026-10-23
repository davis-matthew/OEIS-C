/*
 * A171495  a(n) = 3*a(n-1)+4 for n > 0; a(0) = 6.
 * https://oeis.org/A171495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(4*3^n-1)
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(n) = 3*a(n-1) + 4,  a(0)=6 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* compute 3^n */
    seq_t p = pow3(n);
    /* compute 2*(4*p - 1) */
    seq_t res = 2 * (4 * p - 1);
    return res;
}
