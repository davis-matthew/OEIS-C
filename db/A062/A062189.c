/*
 * A062189  a(n) = 2 * 3^(n-2)*n*(1+2*n).
 * https://oeis.org/A062189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * 3^(n-2) * n * (1 + 2*n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;

    seq_t sn   = (seq_t)n;          /* convert index to seq_t */
    seq_t base = pow3(n - 2);       /* 3^(n-2) */
    seq_t res  = base;

    res *= (seq_t)2;                /* *2            */
    res *= sn;                      /* *n            */
    seq_t term = (seq_t)(1 + (seq_t)(2 * sn)); /* (1+2*n)    */
    res *= term;                    /* *term         */

    return res;
}
