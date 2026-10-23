/*
 * A184903  n+floor(nr/t)+floor(ns/t), where r=(1+sqrt(5))/2, s=r+1, t=r+2.
 * https://oeis.org/A184903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n * r / t) + floor(n * s / t) * with r =
 *            (1+sqrt5)/2, s = r+1, t = r+2. * Using algebraic identities this
 *            reduces to: * a(n) = n + floor( floor(n*sqrt5)/5 ) + floor( (5n
 *            + floor(n*sqrt5)) / 10 )
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the unsigned index to seq_t for arithmetic. */
    seq_t sn = (seq_t)n;

    /* m = floor(n * sqrt5)  == floor( sqrt(5 * n^2) ) */
    seq_t m = floor_sqrt(sn * sn * (seq_t)5);

    /* term1 = floor( m / 5 ) */
    seq_t term1 = m / (seq_t)5;

    /* term2 = floor( (5*n + m) / 10 ) */
    seq_t term2 = (sn * (seq_t)5 + m) / (seq_t)10;

    return sn + term1 + term2;
}
