/*
 * A199626  G.f.:
 *          (1+x)^(2*g)*(1+x^3)^(3*g)/((1-x^2)*(1-x^4))-x^(2*g)*(1+x)^4/((1-x^2)*(1-x^4))
 *          for g=0.
 * https://oeis.org/A199626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = if n is odd : -2*(n+1); if n is even: ((n+3 +
 *            5*(-1)^(n/2))/4) - 2*(n+1)
 * Range:     i8 n <= 64, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if ((n & (idx_t)1) != 0) {                 /* odd n */
        return -( (seq_t)2 * ( (seq_t)n + 1 ) );
    } else {                                   /* even n */
        seq_t k     = (seq_t)(n / 2);
        seq_t sign  = ( (k & 1) == 0 ) ? 1 : -1;   /* (-1)^k */
        seq_t num   = (seq_t)n + 3 + 5 * sign;
        seq_t term  = num / 4;                    /* divisible by 4 */
        return term - 2 * ( (seq_t)n + 1 );
    }
}
