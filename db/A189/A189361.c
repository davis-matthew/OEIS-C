/*
 * A189361  a(n) = n + floor(n*s/r) + floor(n*t/r); r=1, s=sqrt(2), t=sqrt(3).
 * https://oeis.org/A189361
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n*sqrt(3))
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n < 1)
        return 0;
    seq_t tn = (seq_t)n;
    seq_t n2 = tn * tn;
    seq_t f2 = floor_sqrt((seq_t)2 * n2);
    seq_t f3 = floor_sqrt((seq_t)3 * n2);
    return tn + f2 + f3;
}
