/*
 * A005699  a(n) = ceiling(tan(n)^2).
 * https://oeis.org/A005699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceiling(tan(n)^2); for n > 0 this equals floor(sec(n)^2).
 *            tan(n)^2 is computed in long double and the result is converted
 *            to a native integer before it becomes seq_t. For n up to about
 *            10^4 the value stays small (below about 10^6), so it fits easily
 * Range:     u8 n <= 10, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n) {
    long double t, v;
    unsigned long long r;
    if (n == 0) return (seq_t)0;
    t = tanl((long double)n);
    v = ceill(t * t);
    r = (unsigned long long)v;
    return (seq_t)r;
}
