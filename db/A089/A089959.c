/*
 * A089959  a(n) = floor(1/(f(n) - f(n)^2)) with f(n) = frac(n*(sqrt(5) -
 *          1)/2) (fractional part).
 * https://oeis.org/A089959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = floor( 1 / ( {n*k} * (1 - {n*k}) ) ),
 * where k = (sqrt(5) - 1) / 2 and {x} is the fractional part of x.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* k = (sqrt(5)-1)/2 as a long double constant */
    static const long double k = (sqrtl(5.0L) - 1.0L) / 2.0L;

    /* n*k as a long double */
    long double nk = (long double)n * k;

    /* fractional part of nk */
    long double f = nk - floorl(nk);

    /* compute 1/(f*(1-f)) */
    long double prod = f * (1.0L - f);
    long double inv  = 1.0L / prod;

    /* add a tiny epsilon to guard against rounding down by the floor */
    unsigned long long val = (unsigned long long)(floorl(inv + 1e-12L));

    /* cast to the build‑specific seq_t type */
    return (seq_t)val;
}
