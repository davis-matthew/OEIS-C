/*
 * A094500  Least number k such that (n+1)^k / n^k >= 2.
 * https://oeis.org/A094500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = minimal k such that ((n+1)/n)^k >= 2, n≥1
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a floating-point approximation to find an initial guess,
   then corrects it by a few steps so that the result is exact for all n
   that fit into seq_t. */

/* Include required standard headers */
#include <math.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Convert n to long double for accurate calculation */
    long double nd = (long double)n;
    long double ratio = (nd + 1.0L) / nd;   /* (n+1)/n  */
    long double logratio = logl(ratio);      /* log((n+1)/n) */
    long double log2 = 0.693147180559945309417232121458176568075500L; /* ln(2) */

    /* Initial estimate of k = floor( log2 / logratio ) */
    idx_t k = (idx_t)(log2 / logratio);
    if (k == 0)
        k = 1;

    /* Define an absolute tolerance, small compared to 1 but large enough
       to accommodate floating-point rounding errors. */
    const long double eps = 1e-15L;

    /* Helper macro to compute (ratio)^k accurately using powl */
    #define RATIO_POW(kval) powl(ratio, (long double)(kval))

    /* Adjust the estimate downwards if possible */
    while (k > 1 && RATIO_POW(k - 1) >= 2.0L - eps)
        k--;

    /* Adjust upwards if it is still below 2 */
    while (RATIO_POW(k) < 2.0L - eps)
        k++;

    #undef RATIO_POW

    return (seq_t)k;
}
