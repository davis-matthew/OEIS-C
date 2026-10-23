/*
 * A115500  Number of monic irreducible polynomials of degree 1 in
 *          GF(2^n)[x1,x2,x3,x4].
 * https://oeis.org/A115500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n + 4^n + 8^n + 16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = 2^n + 2^(2n) + 2^(3n) + 2^(4n) */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; behavior for n < 1 is undefined. */
    seq_t sum = 0;
    seq_t p1 = pow_uint(2, n);          /* 2^n */
    sum += p1;
    seq_t p2 = pow_uint(2, 2*n);        /* 4^n = 2^(2n) */
    sum += p2;
    seq_t p3 = pow_uint(2, 3*n);        /* 8^n = 2^(3n) */
    sum += p3;
    seq_t p4 = pow_uint(2, 4*n);        /* 16^n = 2^(4n) */
    sum += p4;
    return sum;
}
