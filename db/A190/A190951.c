/*
 * A190951  Closest integer to the largest real number x such that exp(x) =
 *          x^n, for n>=3.
 * https://oeis.org/A190951
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nearest integer to the largest solution x of e^x = x^n
 *            (equivalently x = n·ln x) for n ≥ 3. The value is computed in
 *            double precision and rounded; casting to seq_t is safe since
 *            a(n) < 10^6 for n ≤ 10000
 * Range:     u8 n <= 46, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* values for n < 3 are irrelevant for the problem; return 0 */
    if (n < 3)
        return 0;

    /* start with the asymptotic approximation x ≈ n·ln n */
    double x = (double)n * log((double)n);

    /* Newton iteration for f(x) = x – n·ln x = 0 */
    for (int i = 0; i < 20; ++i) {
        double ln_x = log(x);
        double f    = x - (double)n * ln_x;
        double fp   = 1.0 - (double)n / x;     /* f'(x) */
        double x_new = x - f / fp;
        if (fabs(x_new - x) < 1e-12)   /* convergence */
            break;
        x = x_new;
    }

    /* round to nearest integer and cast to seq_t
       cast through unsigned long long to avoid issues with arbitrary‑precision builds */
    return (seq_t)( (uint64_t)(x + 0.5) );
}
