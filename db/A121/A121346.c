/*
 * A121346  Conjectured lower bound for the number of spheres of radius 1 that
 *          can be packed in a sphere of radius n.
 * https://oeis.org/A121346
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(K*(1 - 2*d)/d^3 + 1/(2*d^2)), where d=1/n and K =
 *            Pi/(3*sqrt(2))
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 1797, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static const long double K = (acosl(-1.0L) / (3.0L * sqrtl(2.0L)));

seq_t a(idx_t n)
{
    /* Offset starts at 2; behaviour for n<2 is undefined by OEIS */
    if (n < 2) return 0;

    /* Work in long double for precision */
    long double nd   = (long double)n;
    long double term = K * (nd - 2.0L) * nd * nd + (nd * nd) / 2.0L;

    /* Floor to integer */
    unsigned long long val = (unsigned long long)floorl(term);

    /* Convert to seq_t (assumes seq_t can be constructed from unsigned long long) */
    return (seq_t)val;
}
