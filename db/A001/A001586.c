/*
 * A001586  Generalized Euler numbers, or Springer numbers.
 * https://oeis.org/A001586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = P_n(1), where P_0(u) = 1 and P_{n+1}(u) = (1+u^2)*P_n'(u)
 *            + u*P_n(u) (d^n/dt^n sec(t) = P_n(tan t)*sec(t); e.g.f.
 *            1/(cos(x)-sin(x)) = sec(x+Pi/4)/sqrt(2)). Coefficients: if P_n =
 *            Sum c_k u^k then P_{n+1} has coefficients d_j = (j+1)*c_{j+1} +
 *            j*c_{j-1}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t len = n + 2;
    seq_t *c = alloc_zero(len);
    seq_t *d = alloc_zero(len);
    seq_t *t;
    seq_t sum = 0;
    idx_t m, j;

    c[0] = 1;
    for (m = 0; m < n; m++) {
        for (j = 0; j <= m + 1; j++) {
            seq_t v = c[j + 1] * (seq_t)(j + 1);
            if (j >= 1) v += c[j - 1] * (seq_t)j;
            d[j] = v;
        }
        t = c; c = d; d = t;
    }
    for (j = 0; j <= n; j++) sum += c[j];

    release(c, len);
    release(d, len);
    return sum;
}
