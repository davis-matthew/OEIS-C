/*
 * A104077  a(n) = round(10000*log(n/10)).
 * https://oeis.org/A104077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = round(10000 * log(n/10))
 * Range:     i8 none, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* Use long double to keep high precision.
       n is uint64_t, so the logarithm is at most ln(2^64)≈44,
       times 10000 is well within 64 bits. */
    const long double SCALE = 10000.0L;
    const long double LOG10 = logl(10.0L);

    long double value = SCALE * (logl((long double)n) - LOG10);
    /* llroundl rounds to nearest integer, halving away from zero. */
    return (seq_t)llroundl(value);
}
