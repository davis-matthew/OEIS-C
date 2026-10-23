/*
 * A104744  Round(1000*x), where x is the solution to x = 5^(n-x).
 * https://oeis.org/A104744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(1000*x), where x satisfies x = 5^(n-x)
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* compute a(n) for OEIS A104744 */
seq_t a(idx_t n)
{
    /* special handling for n = 0 is not required, the Newton solver will
       correctly find the root, but a few initial guesses help convergence   */
    const double ln5 = log(5.0);
    double n_d = (double)n;

    double x;
    if (n == 0)
        x = 0.5;               /* good guess for the smallest root   */
    else
        x = n_d;                /* start at n, usually close enough   */

    /* Newton iterations */
    for (int i = 0; i < 20; ++i)
    {
        double exp_val = exp((n_d - x) * ln5);
        double f = x - exp_val;
        double df = 1.0 + ln5 * exp_val;
        double x_new = x - f / df;
        if (fabs(x_new - x) < 1e-12)
            break;
        x = x_new;
    }

    /* round to the nearest integer after multiplying by 1000 */
    long long y = (long long)floor(x * 1000.0 + 0.5);
    return (seq_t)y;
}
