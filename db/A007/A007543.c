/*
 * A007543  Frequency of n-th largest distance in N times N grid, N > n.
 * https://oeis.org/A007543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n if n is a perfect square, otherwise a(n) = 4*(n -
 *            f(n))*(ceiling(2*sqrt(n)) - n + f(n)), where f(n) =
 *            floor(floor(2*sqrt(n))^2/4)
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   a007543_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007543_isqrt.h"

seq_t a(idx_t n) {
    uint64_t s = a007543_isqrt(n);
    uint64_t fl2, f, ce2, u, v;
    seq_t res;

    if (s * s == n) {
        return (seq_t)2 * (seq_t)n;
    }

    fl2 = a007543_isqrt(4 * n);          /* floor(2*sqrt(n)) */
    f = (fl2 * fl2) / 4;
    ce2 = (fl2 * fl2 == 4 * n) ? fl2 : fl2 + 1; /* ceiling(2*sqrt(n)) */

    u = n - f;
    v = (ce2 + f) - n;

    res = (seq_t)4 * (seq_t)u;
    res *= (seq_t)v;
    return res;
}
