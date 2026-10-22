/*
 * A003441  Number of nonequivalent dissections of a polygon into n triangles
 *          by nonintersecting diagonals rooted at a cell up to rotation.
 * https://oeis.org/A003441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*C(n)/(n+2) + (2/3)*C((n-1)/3), equivalently a(n) =
 *            (C(n+1) - C(n) + 2*C((n-1)/3))/3, where C = A000108 (Catalan)
 *            and C(x) = 0 for nonintegral x. The division is done with
 *            quotient and remainder so no intermediate exceeds about C(n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a3441_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a3441_g.h"

seq_t a(idx_t n) {
    int has = ((n - 1) % 3 == 0);
    idx_t k = (n - 1) / 3;
    seq_t c = 1;            /* C(0) */
    seq_t ck = 0;
    if (has && k == 0) ck = 1;
    for (idx_t i = 0; i < n; i++) {
        /* C(i+1) = C(i) * 2(2i+1) / (i+2), reduced to avoid overflow */
        uint64_t m = (uint64_t)i + 2;
        uint64_t r = (uint64_t)(c % m);
        uint64_t g = a3441_g(m, r);
        uint64_t mul = (2 * (2 * (uint64_t)i + 1)) / (m / g);
        c = (c / g) * mul;
        if (has && i + 1 == k) ck = c;
    }
    /* c = C(n) */
    uint64_t d = (uint64_t)n + 2;
    seq_t q = c / d;
    uint64_t r = (uint64_t)(c % d);
    seq_t t = ck * 2;
    seq_t q2 = t / 3;
    uint64_t r2 = (uint64_t)(t % 3);
    uint64_t small = (3 * (uint64_t)n * r + d * r2) / (3 * d);
    seq_t res = q * n;
    res += q2;
    res += small;
    return res;
}
