/*
 * A140232  a(n) = ceiling(n*exp((1+sqrt(5))/2)).
 * https://oeis.org/A140232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*exp((1+sqrt(5))/2))
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* precomputed constant exp((1+sqrt(5))/2) with long double precision */
    static const long double C = expl((1.0L + sqrtl(5.0L)) / 2.0L);

    /* compute n*C in long double, then ceil and cast to integer */
    long double val = (long double)n * C;
    unsigned long long result = (unsigned long long)ceill(val);

    return (seq_t)result;
}
