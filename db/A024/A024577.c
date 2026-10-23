/*
 * A024577  a(n) = [ n/{n/e} ], {x} := x - [ x ].
 * https://oeis.org/A024577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n / {n/e} ) where {x} = x - floor(x)
 * Range:     u8 n <= 29, u16 n <= 192, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n must be >= 1; behavior for n==0 is undefined, returned 0. */
    if (n < 1)
        return 0;

    /* use long double arithmetic to maintain high precision */
    const long double inv_e = 1.0L / expl(1.0L);
    const long double rn   = (long double)n;
    const long double q    = rn * inv_e;          /* n/e          */
    const idx_t      m     = (idx_t)q;            /* floor(n/e)   */
    const long double f    = q - (long double)m; /* {n/e}       */
    const long double val  = rn / f;             /* n/{n/e}     */
    const long double eps  = 1e-14L;              /* safety margin */

    /* Convert to integer result.  The cast to unsigned long long
       is safe because the sequence values that fit in 64 bits are
       guaranteed to be < ULLONG_MAX for the tested range. */
    return (seq_t)( (unsigned long long)floorl(val + eps) );
}
