/*
 * A176177  a(n) = 2*n*3^(n-1) - (3^n-1)/2.
 * https://oeis.org/A176177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*3^(n-1) - (3^n-1)/2
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=8, a(n)=7*a(n-1)-15*a(n-2)+9*a(n-3) for n>2 */
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t p2 = pow3(n);            /* 3^n */
    seq_t p1 = p2 / 3;             /* 3^(n-1) */
    seq_t term1 = 2 * (seq_t)n * p1;
    seq_t term2 = (p2 - 1) / 2;
    return term1 - term2;
}
