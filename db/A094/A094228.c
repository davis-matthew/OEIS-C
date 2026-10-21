/*
 * A094228  Let s = -sqrt(2)*sqrt(n)*sqrt(1+I*n/(2*Pi))-n*log(n); then a(n) =
 *          floor(Re(-s)).
 * https://oeis.org/A094228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( sqrt(2)*sqrt(n)*sqrt((sqrt(1+(n/(2π))²)+1)/2) +
 *            n*log(n) ) (obtained from the definition a(n)=floor(Re(-s)) with
 *            s = -√2√n√(1+I n/(2π)) - n log(n))
 * Range:     u8 n <= 57, u16 n <= 7074, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* for long double math functions */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1 – values for n≥1 only, but guard */
        return (seq_t)0;

    /* use long double for the intermediate calculations */
    const long double pi   = acosl(-1.0L);          /* π */
    const long double nn   = (long double) n;      /* n as long double */

    /* x = n/(2π) */
    long double x = nn / (2.0L * pi);

    /* Re( sqrt(1 + i*x) ) = sqrt( (sqrt(1+x²)+1)/2 ) */
    long double sq = sqrtl(1.0L + x * x);          /* sqrt(1+x²) */
    long double re = sqrtl( (sq + 1.0L) / 2.0L );   /* real part */

    /* sqrt(2) * sqrt(n) * re + n*log(n) */
    long double val = sqrtl(2.0L) * sqrtl(nn) * re + nn * logl(nn);

    /* floor, add a tiny epsilon to avoid rounding toward the
       next integer when val is very close to an integer. */
    long double floored = floorl(val + 1e-12L);

    /* cast to seq_t via an intermediate long long; safe for n up to ≈10⁶ */
    return (seq_t)(long long)floored;
}
