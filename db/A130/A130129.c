/*
 * A130129  a(n) = (3*n+1)*2^n.
 * https://oeis.org/A130129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)*2^n
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 25, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t power = pow2(n);                     /* compute 2^n */
    seq_t factor = ((seq_t)3 * (seq_t)n) + 1;   /* compute 3*n + 1  */
    return power * factor;                     /* (3*n+1)*2^n      */
}
