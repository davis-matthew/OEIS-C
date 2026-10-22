/*
 * A004238  a(n) = 100*log(n) rounded to nearest integer.
 * https://oeis.org/A004238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(100*log(n))
 * Range:     u8 n <= 12, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double x = 100.0L * logl((long double)n);
    long long r = (long long)floorl(x + 0.5L);
    return (seq_t)r;
}
