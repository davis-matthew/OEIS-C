/*
 * A190053  a(n) = n + [n*s/r] + [n*t/r]; r=2, s=sin(Pi/3), t=csc(Pi/3).
 * https://oeis.org/A190053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(3)/4) + floor(n / sqrt(3)) Equivalent
 *            integer form: a(n) = n + floor(sqrt(3*n*n)/4) +
 *            floor(sqrt(3*n*n)/3)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the utility function: floor_sqrt(seq_t x)  from "../utils/floor_sqrt.h" */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* compute t = 3 * n * n safely in seq_t */
    seq_t t = (seq_t)n;
    t *= t;          /* t = n*n   */
    t *= (seq_t)3;   /* t = 3*n*n */

    /* s = floor(sqrt(t)) = floor(sqrt(3*n*n)) */
    seq_t s = floor_sqrt(t);

    /* a(n) = n + floor(s/4) + floor(s/3) */
    seq_t res = (seq_t)n;
    res += s / (seq_t)4;
    res += s / (seq_t)3;
    return res;
}
