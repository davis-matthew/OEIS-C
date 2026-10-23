/*
 * A161320  Floor of Sum_{k=1..n} Sum_{m=1..k} (2*cosh(Pi*m/(2*k)) - 1).
 * https://oeis.org/A161320
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 2*Σ_{k=1..n} Σ_{m=1..k} cosh(π*m/(2*k)) -
 *            n*(n+1)/2 )
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 1 (a(1) = 4) */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* protect against n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* constants */
    static const long double pi  = acosl(-1.0L);
    static const long double C   = expl(pi / 2.0L);      /* e^(π/2)   */
    static const long double invC = 1.0L / C;           /* e^(-π/2)  */
    static const long double D    = C - 1.0L;           /* C - 1     */

    long double sum = 0.0L;
    for (idx_t k = 1; k <= n; ++k)
    {
        long double ak = pi / (2.0L * (long double)k); /* π / (2k)   */
        long double R  = expl(ak);                     /* e^(ak)     */
        long double term = D * (R + invC) / (R - 1.0L) - (long double)k;
        sum += term;
    }

    /* add a tiny safety margin before flooring */
    long double eps = 1e-12L;
    uint64_t val = (uint64_t)floorl(sum + eps);

    return (seq_t)val;
}
