/*
 * A004236  a(n) = ceiling(10*log(n)).
 * https://oeis.org/A004236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(10*log(n)), computed as an integer from a long
 *            double logarithm of the index
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    long double x;
    long long k;
    if (n <= 1) return (seq_t)0;
    x = 10.0L * logl((long double)n);
    k = (long long)ceill(x);
    return (seq_t)(unsigned long long)k;
}
