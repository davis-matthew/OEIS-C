/*
 * A004235  a(n) = 10*log(n) rounded to the nearest integer.
 * https://oeis.org/A004235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(10*log(n)); computed in floating point on the index
 *            n only, since the result is small (< 450)
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double v = 10.0L * logl((long double)n);
    long long r = (long long)floorl(v + 0.5L);
    if (r < 0) r = 0;
    return (seq_t)r;
}
