/*
 * A085534  a(n) = (2n)^(2n).
 * https://oeis.org/A085534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n)^(2*n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* The following function is part of the utils library,
   we can simply use the operators defined for seq_t. */
seq_t a(idx_t n)
{
    if (n == 0)   /* 0^0 is defined as 1 in this sequence */
        return (seq_t)1;

    seq_t base  = (seq_t)(2 * n);
    idx_t  exp  = 2 * n;
    seq_t  res  = (seq_t)1;

    /* exponentiation by squaring, works for any seq_t type */
    while (exp > 0) {
        if (exp & 1)
            res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}
