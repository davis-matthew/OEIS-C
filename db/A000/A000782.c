/*
 * A000782  a(n) = 2*Catalan(n) - Catalan(n-1).
 * https://oeis.org/A000782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n - 5)/(n + 1) * C(n-1), where C(k) = A000108(k) is
 *            computed by C(0) = 1, C(k) = C(k-1) * 2*(2*k-1)/(k+1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   a782_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a782_gcd.h"

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    uint64_t m, d, g;
    for (k = 1; k + 1 <= n; k++) {
        m = 2 * (2 * (uint64_t)k - 1);
        d = (uint64_t)k + 1;
        g = a782_gcd(m, d);
        m /= g;
        d /= g;
        c = c / d;
        c = c * m;
    }
    m = 7 * (uint64_t)n - 5;
    d = (uint64_t)n + 1;
    g = a782_gcd(m, d);
    m /= g;
    d /= g;
    c = c / d;
    return c * m;
}
