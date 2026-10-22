/*
 * A004226  a(n) = 1000*log_10(n) rounded to the nearest integer.
 * https://oeis.org/A004226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(1000*log_10(n)); log is taken of the index in long
 *            double, then the rounded result is converted to seq_t as an
 *            integer
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double v = 1000.0L * log10l((long double)n);
    long long r = (long long)floorl(v + 0.5L);
    return (seq_t)r;
}
