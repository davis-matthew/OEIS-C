/*
 * A004239  a(n) = ceiling(100*log(n)).
 * https://oeis.org/A004239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(100*log(n)), with log the natural logarithm; the
 *            float math is done on the index only and gives a small integer
 * Range:     u8 n <= 12, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    long long k;
    long double x;
    if (n <= 1)
        return (seq_t)0;
    x = 100.0L * logl((long double)n);
    k = (long long)ceill(x);
    return (seq_t)k;
}
