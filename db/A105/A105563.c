/*
 * A105563  a(n) = if (exactly 4 Fibonacci numbers exist with exactly n
 *          digits) then 1, otherwise 0.
 * https://oeis.org/A105563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if exactly 4 Fibonacci numbers have n decimal digits,
 *            else 0. Computation uses the identity a(n) = 1 - A105565(n),
 *            where the number of Fibonacci numbers with n digits is floor( (n
 *            + log10(sqrt(5))) / log10(phi) ) - floor( (n-1 + log10(sqrt(5)))
 *            / log10(phi) ). If that difference equals 4...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>          /* log10l, sqrtl, floorl */

static const long double LOG10_PHI   = log10l((1.0L + sqrtl(5.0L)) / 2.0L);
static const long double LOG10_SQRT5 = 0.5L * log10l(5.0L);
static const long double EPS         = 1e-12L;

seq_t a(idx_t n)
{
    if (n < 1)                /* offset not defined below 1 */
        return 0;

    if (n == 1)               /* a(1) is explicitly 0 */
        return 0;

    /* Convert n to long double for high‑precision computation */
    long double nd = (long double)n;

    /* Compute the two floor values that count the Fibonacci indices
       having exactly n decimal digits. */
    long double t1 = (nd + LOG10_SQRT5) / LOG10_PHI + EPS;
    long double t2 = ((nd - 1.0L) + LOG10_SQRT5) / LOG10_PHI + EPS;

    long long floorN = (long long)floorl(t1);
    long long floorM = (long long)floorl(t2);

    long long count = floorN - floorM;

    return (seq_t)(count == 4 ? 1 : 0);
}
