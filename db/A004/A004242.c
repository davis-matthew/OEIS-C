/*
 * A004242  a(n) = ceiling(1000*log(n)).
 * https://oeis.org/A004242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(1000*log(n))
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    long double x = 1000.0L * logl((long double)n);
    long long r = (long long)ceill(x);
    seq_t res = (seq_t)r;
    return res;
}
