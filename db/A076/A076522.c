/*
 * A076522  a(n) = floor( n*log_2(4*n/3) ).
 * https://oeis.org/A076522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( n*log_2(4*n/3) )
 * Range:     u8 n <= 43, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* The function is defined for n ≥ 1.  For n < 1 the returned value
 * is not defined by OEIS, but we return 0 for safety. */
seq_t a(idx_t n)
{
    if (n < 1u) return 0;

    /* Compute the expression using long double to keep high precision. */
    long double x = (long double)4 * (long double)n / 3.0L;
    long double v = (long double)n * log2l(x);

    /* Add a tiny epsilon to guard against the rare case where the
     * rounding error would move the value just below an integer. */
    const long double eps = 1e-12L;
    uint64_t r = (uint64_t)(v + eps);

    return (seq_t)r;
}
