/*
 * A002596  Numerators in expansion of sqrt(1+x). Absolute values give
 *          numerators in expansion of sqrt(1-x).
 * https://oeis.org/A002596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, frac, sign
 * Formula:   a(0) = 1; for n >= 1, a(n) = (-1)^(n+1) * (odd part of C(n-1)),
 *            where C(m) = A000108(m) is the m-th Catalan number. C(k) =
 *            C(k-1)*(4k-2)/(k+1), so odd(C(k)) = odd(C(k-1))*(2k-1)/odd(k+1)
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 21, i64 n <= 37, i128 n <= 71,
 *            big any n
 * Helpers:   a002596_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002596_g.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t m = n - 1;
    seq_t v = 1; /* odd part of C(k) */
    for (idx_t k = 1; k <= m; k++) {
        uint64_t d = (uint64_t)k + 1;
        while ((d & 1) == 0) d >>= 1;
        uint64_t r = (uint64_t)(long long)(v % (seq_t)d);
        uint64_t g = a002596_g(d, r);
        v /= (seq_t)g;
        d /= g;
        uint64_t t = (2 * (uint64_t)k - 1) / d;
        v *= (seq_t)t;
    }
    if (n % 2 == 0) v = -v;
    return v;
}
