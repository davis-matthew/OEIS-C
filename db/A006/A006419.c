/*
 * A006419  a(n) = 2^(2*n+1) - C(2*n+3,n+1) + C(2*n+1,n).
 * https://oeis.org/A006419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + A000245(n), a(0) = 0, where A000245(k+1) =
 *            A000245(k)*(2k+1)*(2k+2)/(k*(k+3)), A000245(1) = 1 (from G.f.
 *            c(x)^3*x/(1-4x))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   a006419_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006419_gcd.h"

seq_t a(idx_t n) {
    seq_t s = 0;   /* a(k) */
    seq_t t = 1;   /* [x^k] c(x)^3 = A000245(k+1) */
    idx_t k;
    if (n == 0) return s;
    for (k = 1; k <= n; k++) {
        if (k >= 2) {
            uint64_t m = (uint64_t)(k - 1);
            uint64_t p = 2 * m + 1, q = 2 * m + 2, d = m * (m + 3);
            uint64_t g = a006419_gcd(p, d);
            p /= g; d /= g;
            g = a006419_gcd(q, d);
            q /= g; d /= g;
            t = t / d;
            t = t * p;
            t = t * q;
        }
        s = s * 4 + t;
    }
    return s;
}
