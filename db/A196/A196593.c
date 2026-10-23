/*
 * A196593  Number of fixed tree-like convex polyominoes.
 * https://oeis.org/A196593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+2) - (n^3 - n^2 + 10*n + 4)/2
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* values for n < 1 are undefined */

    seq_t nn = (seq_t)n;                       /* cast to seq_t for safety */
    seq_t poly = nn * nn * nn                 /* n^3          */
              - nn * nn                       /* - n^2        */
              + (seq_t)10 * nn                /* + 10*n       */
              + (seq_t)4;                     /* + 4           */

    seq_t half = poly / (seq_t)2;              /* divide by 2   */

    seq_t power = pow2((idx_t)(n + 2));        /* 2^(n+2)      */

    return power - half;                       /* final result  */
}
