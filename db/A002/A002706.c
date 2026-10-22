/*
 * A002706  Theta series of 6-dimensional lattice A_6^(2) (other names for
 *          this lattice or the corresponding quadratic form are
 *          LAMBDA_{3,lambda}, P_6^(5), phi_6, F_14).
 * https://oeis.org/A002706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (49*F(n) - 7*S(n) - 42*c(n))/8 for n >= 1, a(0) = 1,
 *            where chi(m) = Kronecker(-7,m), S(n) = Sum_{d|n} chi(d)*d^2,
 *            F(n) = Sum_{d|n} chi(n/d)*d^2, and c(n) = [q^n]
 *            eta(q)^3*eta(q^7)^3 (= A002656(n)); equivalently a(n) =
 *            A002653(n) - 6*A002656(n)
 * Range:     u8 n <= 5, u16 n <= 93, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   a002706_chi, a002706_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002706_chi.h"
#include "../utils/a002706_isqrt.h"

seq_t a(idx_t n) {
    __int128 S = 0, F = 0, C = 0, r;
    uint64_t d, m, k;
    if (n == 0) return 1;

    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            uint64_t e = n / d;
            __int128 d2 = (__int128)d * d, e2 = (__int128)e * e;
            S += a002706_chi(d) * d2;
            F += a002706_chi(e) * d2;
            if (e != d) {
                S += a002706_chi(e) * e2;
                F += a002706_chi(d) * e2;
            }
        }
    }

    /* eta(q)^3 = Sum_k (-1)^k (2k+1) q^{k(k+1)/2}; product has leading q^1 */
    m = n - 1;
    for (k = 0; k * (k + 1) / 2 <= m; k++) {
        uint64_t rest = m - k * (k + 1) / 2, t, s, j;
        if (rest % 7) continue;
        t = rest / 7;
        s = a002706_isqrt(8 * t + 1);
        if (s * s != 8 * t + 1) continue;
        j = (s - 1) / 2;
        {
            __int128 term = (__int128)(2 * k + 1) * (__int128)(2 * j + 1);
            if ((k + j) & 1) C -= term; else C += term;
        }
    }

    r = (49 * F - 7 * S - 42 * C) / 8;
    return (seq_t)(unsigned long long)r;
}
