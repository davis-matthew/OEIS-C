/*
 * A105951  a(2*n) = -(2^(2*n+1) + 1), a(2*n+1) = (2^(n+1) - (-1)^n)^2.
 * https://oeis.org/A105951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(2*n) = -(2^(2*n+1)+1), a(2*n+1) = (2^(n+1)-(-1)^n)^2
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 61, i128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {                 /* n even: n = 2*k  */
        idx_t k = n >> 1;
        seq_t p = pow2(2 * k + 1);      /* 2^(2*k+1)  */
        return -(p + 1);
    } else {                            /* n odd: n = 2*k+1*/
        idx_t k = (n - 1) >> 1;
        seq_t p = pow2(k + 1);          /* 2^(k+1)    */
        seq_t sign = (k & 1) ? -1 : 1;  /* (-1)^k     */
        seq_t t = p - sign;             /* 2^(k+1) - (-1)^k */
        return t * t;                   /* square       */
    }
}
