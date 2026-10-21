/*
 * A212448  Floor(4n + log(4n)).
 * https://oeis.org/A212448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4*n + log(4*n))
 * Range:     u8 n <= 62, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 4*n and its natural logarithm using long double */
    long double x = (long double)4 * (long double)n;
    long double l = logl(x);
    long double v = x + l;

    /* floor with a tiny epsilon to guard against rounding errors */
    uint64_t val = (uint64_t)floorl(v + 1e-12L);

    /* result fits in seq_t via implicit conversion from uint64_t */
    return (seq_t)val;
}
