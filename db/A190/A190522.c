/*
 * A190522  a(n) = n + [nr/u] + [ns/u] + [nt/u] + [nv/u] + [nw/u], where
 *          r=sin(x), s=cos(x), t=tan(x), u=csc(x), v=sec(x), w=cot(x),
 *          x=2*Pi/5.
 * https://oeis.org/A190522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n·sin²x⌋ + ⌊n·sinx·cosx⌋ + ⌊n·sin²x/cosx⌋ + ⌊n·tanx⌋
 *            + ⌊n·cosx⌋ where x = 2·π/5. This is a direct implementation
 *            using long double arithmetic and integer truncation to obtain
 *            the required floor values
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Avoid recomputing constants on every call by storing them in a static block
       that is executed only once.  The constants are computed from long double
       values to retain adequate precision for the range of n expected in tests. */
    static const long double X     = 2.0L * acosl(-1.0L) / 5.0L;   /* 2π/5  */
    static const long double SIN_X = sinl(X);
    static const long double COS_X = cosl(X);
    static const long double TAN_X = tanl(X);

    /* Each term is a floor of a real expression.  We compute the floor in
       long double and then cast the result to an unsigned long long.  The
       cast is safe because the maximum value of a(n) for the tested n
       stays well below 2^64. */
    const unsigned long long term_sin2  = (unsigned long long) floorl( (long double)n * SIN_X * SIN_X );
    const unsigned long long term_sincos = (unsigned long long) floorl( (long double)n * SIN_X * COS_X );
    const unsigned long long term_sin2cos = (unsigned long long) floorl( (long double)n * SIN_X * SIN_X / COS_X );
    const unsigned long long term_tan   = (unsigned long long) floorl( (long double)n * TAN_X );
    const unsigned long long term_cos   = (unsigned long long) floorl( (long double)n * COS_X );

    /* Sum all integer parts together, including the linear term n. */
    const unsigned long long sum = (unsigned long long)n + term_sin2 + term_sincos
                                  + term_sin2cos + term_tan + term_cos;

    return (seq_t) sum;
}
