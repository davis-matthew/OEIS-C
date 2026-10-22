/*
 * A007904  Crystal ball sequence for diamond.
 * https://oeis.org/A007904
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (10*n^3 + 15*n^2 + 26*n + 12)/12 for n even, (10*n^3 +
 *            15*n^2 + 26*n + 9)/12 for n odd (closed form of G.f.
 *            (1+2x+4x^2+2x^3+x^4)/((1-x)^4*(1+x))). With n = 2m: a =
 *            m*(20m^2+13)/3 + 5m^2 + 1; n = 2m+1: a = m*(20m^2+43)/3 + 15m^2
 *            + 5
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   third_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/third_prod.h"

seq_t a(idx_t n) {
    seq_t m = (seq_t)(n / 2);
    seq_t m2 = m * m;
    if ((n & 1) == 0) {
        return third_prod(m, 20 * m2 + 13) + 5 * m2 + 1;
    } else {
        return third_prod(m, 20 * m2 + 43) + 15 * m2 + 5;
    }
}
