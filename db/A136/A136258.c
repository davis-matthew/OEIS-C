/*
 * A136258  a(n) = 2*a(n-1) - 2*a(n-2), with a(0)=1, a(1)=5.
 * https://oeis.org/A136258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-2), a(0)=1, a(1)=5
 * Range:     i8 n <= 9, i16 n <= 25, i32 n <= 57, i64 n <= 121, i128 n <= 249,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t k = n / 4;          /* exponent for (-4) */
    idx_t r = n % 4;          /* determines the coefficient */

    seq_t power = seq_pow((seq_t)-4, k);  /* (-4)^k in seq_t */

    switch (r)
    {
        case 0: return power;                     /* a(4k)   = (-4)^k          */
        case 1: return (seq_t)5 * power;          /* a(4k+1) = 5*(-4)^k        */
        case 2: return (seq_t)8 * power;          /* a(4k+2) = 8*(-4)^k        */
        default: return (seq_t)6 * power;         /* a(4k+3) = 6*(-4)^k        */
    }
}
