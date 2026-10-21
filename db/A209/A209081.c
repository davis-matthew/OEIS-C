/*
 * A209081  Floor(A152170(n)/n^n). Floor of the expected value of the
 *          cardinality of the image of a function from [n] to [n].
 * https://oeis.org/A209081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n - n*(1 - 1/n)^n) This equals floor(n -
 *            floor((n-1)^n / n^(n-1))) for n > 1. We compute the fractional
 *            part with long double precision and then obtain the integer
 *            result
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;

    /* Compute n * ( (n-1)/n )^n using long double for high precision */
    long double base = (long double)(n - 1) / (long double)n;
    long double power = powl( base, (long double)n );
    long double val   = (long double)n * power;

    /* Floor of the value.  Add a tiny epsilon to guard against
       round‑off that could push a value just below an integer
       above that integer. */
    unsigned long long flo = (unsigned long long)floorl( val + 1e-12L );

    /* a(n) = n - 1 - floor(n * ((n-1)/n)^n)  */
    return (seq_t)( n - 1 - flo );
}
