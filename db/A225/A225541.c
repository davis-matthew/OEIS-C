/*
 * A225541  Floor(W(n)) where W is the Lambert W function.
 * https://oeis.org/A225541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(W(n)), where W solves W*exp(W) = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* The Lambert W function W(n) satisfies W(n)*exp(W(n)) = n.
       For n = 0 or 1, W(n) = 0, so the floor is 0. */
    if (n <= 1)
        return 0;

    /* Use a simple Newton iteration with a logarithmic initial guess. */
    double dn = (double)n;
    double x  = log(dn);               /* initial approximation */
    for (int i = 0; i < 10; ++i)       /* 10 iterations are enough for n ≤ 10000 */
    {
        double e  = exp(x);
        double fx = x * e - dn;
        double dfx = e * (x + 1.0);     /* derivative of x*exp(x) = exp(x)*(x+1) */
        x -= fx / dfx;
    }

    /* The floor of the (possibly slightly off) result.  A tiny epsilon
       guards against rounding errors that would otherwise push the value
       just below an integer. */
    long long r = (long long)floor(x + 1e-12);
    if (r < 0) r = 0;
    return (seq_t)r;
}
