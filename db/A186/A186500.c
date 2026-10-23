/*
 * A186500  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=i^2 and g(j)=-4+5j^2. Complement
 *          of A186499.
 * https://oeis.org/A186500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor( sqrt( 5*n^2 - 7/2 ) )
 * Range:     u8 n <= 3, u16 n <= 28, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/isqrt_seq.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t first */
    seq_t nn   = (seq_t)n;
    /* compute 10*n^2 - 7  (the numerator of 5*n^2 - 7/2) */
    seq_t num  = (seq_t)10 * nn * nn - (seq_t)7;
    /* integer divide by 2 (floor of the rational value) */
    seq_t half = num / (seq_t)2;
    /* floor of the square root of the integer */
    seq_t k    = isqrt_seq(half);
    /* final formula */
    return nn + k;
}
