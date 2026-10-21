/*
 * A072353  a(n) is the index of the largest Fibonacci number containing n
 *          digits.
 * https://oeis.org/A072353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = floor(n / log10(phi) + log10(sqrt(5))/log10(phi))
 * Range:     u8 n <= 53, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = (1+sqrt(5))/2  (golden ratio) */

#include <math.h>
#include <stdint.h>

static const double LOG10_PHIS = log10((1.0 + sqrt(5.0)) / 2.0);
static const double INV_LOG10_PHIS = 1.0 / LOG10_PHIS;
static const double LOG10_SQRT5_OVER_LOG10_PHIS = log10(sqrt(5.0)) * INV_LOG10_PHIS;

seq_t a(idx_t n)
{
    /* The formula yields an integer less than or equal to the desired index.
       A small epsilon is added to guard against floating‑point rounding
       errors that could push a value like 6.0000000‑000001 to 5.999… */
    const double eps = 1e-12;
    double d = (double)n * INV_LOG10_PHIS + LOG10_SQRT5_OVER_LOG10_PHIS + eps;
    idx_t m = (idx_t)floor(d);
    return (seq_t)m;
}
