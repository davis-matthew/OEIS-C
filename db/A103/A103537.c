/*
 * A103537  Round(1000*x), where x is the solution to x = 3^(n-x).
 * https://oeis.org/A103537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(1000*x), where x satisfies x = 3^(n-x)
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Solve x = 3^(n-x) by Newton‑Raphson on the transformed equation
       c*L + e^L = n, where L = ln(x) and c = 1/ln(3). */
    double ln3   = log(3.0);          /* ln(3) */
    double invln3 = 1.0 / ln3;        /* 1/ln(3) */

    /* Initial guess for L:
       L ≈ ln(n) for n > 0, 0 for n == 0. */
    double L = (n > 0) ? log((double)n) : 0.0;

    /* Perform a few Newton iterations until convergence. */
    for (int i = 0; i < 80; ++i)
    {
        double eL   = exp(L);
        double g    = eL + invln3 * L - (double)n;   /* f(L) */
        if (fabs(g) < 1e-12)            /* adequate precision for rounding */
            break;
        double dg   = eL + invln3;      /* f'(L) */
        double Lnew = L - g / dg;
        if (fabs(Lnew - L) < 1e-14)     /* convergence */
        {
            L = Lnew;
            break;
        }
        L = Lnew;
    }

    /* Recover x = e^L and compute 1000*x.  All intermediate work is in
       double, which gives far more than enough precision for the final
       integer rounding. */
    double x   = exp(L);
    double val = x * 1000.0;

    /* Convert to integer once, then cast to seq_t.  The cast to
       (uint64_t) ensures the compiler can construct seq_t from an
       integer, which is valid for all supported builds. */
    uint64_t tmp = (uint64_t)(val + 0.5);   /* round to nearest integer */
    return (seq_t)tmp;
}
