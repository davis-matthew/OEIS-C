/*
 * A000503  a(n) = floor(tan(n)).
 * https://oeis.org/A000503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(n) = floor(tan(n)), with tan evaluated in long double; the
 *            result is an integer that fits in a long long
 * Range:     i8 n <= 10, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double t = tanl((long double)n);
    long long r = (long long)floorl(t);
    return (seq_t)r;
}
