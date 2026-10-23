/*
 * A121282  a(n) = ceiling(n*Pi*e).
 * https://oeis.org/A121282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  less, nonn, easy
 * Formula:   a(n) = ceil(n*Pi*e)
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <limits.h>

static const long double PI  = 3.141592653589793238462643383279502884L;
static const long double E   = 2.718281828459045235360287471352662497L;

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Compute n * Pi * e with long double precision */
    long double prod = (long double)n * PI * E;

    /* Subtract a tiny epsilon to guard against rounding errors that
       might push the value slightly below an integer.  */
    const long double eps = 1e-14L;

    /* Convert to unsigned long long before casting to seq_t.  */
    unsigned long long val = (unsigned long long)(ceill(prod - eps));
    return (seq_t)val;
}
