/*
 * A002420  Expansion of sqrt(1 - 4*x) in powers of x.
 * https://oeis.org/A002420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, nice, easy
 * Formula:   a(0) = 1, a(n) = -2*C(n-1) for n >= 1, where C(k) = A000108(k)
 *            is the Catalan number, computed by C(0) = 1, C(k+1) =
 *            C(k)*2*(2k+1)/(k+2); this equals a(n) = binomial(2n,n)/(1-2n)
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 19, i64 n <= 36, i128 n <= 68,
 *            big any n
 * Helpers:   a002420_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002420_g.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t c = 1;
    for (idx_t k = 0; k + 1 < n; k++) {
        uint64_t num = 2 * (2 * (uint64_t)k + 1);
        uint64_t den = (uint64_t)k + 2;
        uint64_t g = a002420_g(num, den);
        num /= g;
        den /= g;
        c = c / (seq_t)den * (seq_t)num;
    }
    return -(c + c);
}
