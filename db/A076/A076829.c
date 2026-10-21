/*
 * A076829  a(n) = floor(log(log(n^n^n))).
 * https://oeis.org/A076829
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*log(n)+log(log(n)))
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* For indices below the offset, the value is irrelevant. */
    if (n < 2)
        return 0;

    /* Compute the expression in double precision.
       The error is far below one (≈10⁻¹⁵) for n ≤ 10⁴, so the
       result of floor is exact. */
    double d = (double)n;
    double val = d * log(d) + log(log(d));

    /* Convert to integer.  The tiny epsilon prevents rounding
       errors that could occur exactly on integer boundaries. */
    long long r = (long long)floor(val + 1e-12);

    return (seq_t)r;
}
