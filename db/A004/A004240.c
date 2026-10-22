/*
 * A004240  a(n) = floor(1000*log(n)).
 * https://oeis.org/A004240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1000*log(n))
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double v = 1000.0L * logl((long double)n);
    long long k = (long long)floorl(v);
    /* guard against tiny rounding error at integer boundaries */
    if (k < 0) k = 0;
    return (seq_t)k;
}
