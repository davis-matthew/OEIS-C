/*
 * A164082  Rounded value of 2^(n-1) times the surface area of the unit sphere
 *          in 2n-dimensional space.
 * https://oeis.org/A164082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(((2*Pi)^n)/(n-1)!)
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const long double PI = 3.141592653589793238462643383279502884L;
static const long double TWO_PI = 2.0L * PI;

seq_t a(idx_t n)
{
    /* The definition uses the formula for n ≥ 1.  For n > 18
       (2π)^n/(n-1)! is already < 0.5, so the rounded value is 0. */
    if (n > 18u) return (seq_t)0;

    /* Compute (2π)^n */
    long double pow_val = 1.0L;
    for (idx_t i = 0; i < n; ++i)
        pow_val *= TWO_PI;

    /* Compute (n-1)! */
    long double fact = 1.0L;
    for (idx_t i = 2; i < n; ++i)
        fact *= (long double)i;

    /* Evaluate the expression and round to nearest integer. */
    long double value = pow_val / fact;
    uint64_t rounded = (uint64_t)(value + 0.5L);

    return (seq_t)rounded;
}
