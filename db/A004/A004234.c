/*
 * A004234  a(n) = floor(10*log(n)).
 * https://oeis.org/A004234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(10*log(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    /* floating point is applied to the index only; the result is a small integer */
    long double v = 10.0L * logl((long double)n);
    unsigned long long k = (unsigned long long)floorl(v);
    return (seq_t)k;
}
