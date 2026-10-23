/*
 * A046980  Numerators of Taylor series for exp(x)*cos(x).
 * https://oeis.org/A046980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac, easy, nice
 * Formula:   a(n) = 1, 1, 0, -1, -1, -1, 0, 1 repeating with period 8
 *            (derived from G.f. (1+x-x^3)/(1+x^4))
 * Range:     i8 n <= 92, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The recurrence a(n+4) = -a(n) with initial values
       a(0)=1, a(1)=1, a(2)=0, a(3)=-1 produces the periodic pattern
       [1, 1, 0, -1, -1, -1, 0, 1].  We return the value for n mod 8. */
    static const seq_t pattern[8] = {1, 1, 0, -1, -1, -1, 0, 1};
    return pattern[n & 7U];
}
