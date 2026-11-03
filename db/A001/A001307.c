/*
 * A001307  Expansion of 1/(1-x)^2/(1-x^2)/(1-x^4)/(1-x^10)/(1-x^20).
 * https://oeis.org/A001307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1-x)^2 (1-x^2) (1-x^4) (1-x^10) (1-x^20)). a(n) =
 *            Sum_{i,j>=0, 20i+10j<=n} g(n-20i-10j), where g(m) = [x^m]
 *            1/((1-x)^2(1-x^2)(1-x^4)) = Sum_{c=0..floor(m/4)}
 *            (k-2c+1)(k-2c+1+r), with m = 2k+r, evaluated in closed form
 * Range:     u8 n <= 18, u16 n <= 86, u32 n <= 943, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 66 terms through n = 825 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t a001307_g(uint64_t m) {
    uint64_t k = m / 2, r = m % 2;
    uint64_t T = k + 1;
    uint64_t C = k / 2;
    uint64_t s = (T % 2 == 0) ? 2 : 1;
    uint64_t cc = C * (C + 1);
    uint64_t S1 = (C + 1) * s + cc;
    uint64_t S2 = (C + 1) * s * s + 2 * s * cc + 2 * (cc * (2 * C + 1) / 3);
    return S2 + r * S1;
}

seq_t a(idx_t n) {
    seq_t res = 0;
    for (idx_t i = 0; 20 * i <= n; i++) {
        idx_t rem = n - 20 * i;
        for (idx_t j = 0; 10 * j <= rem; j++) {
            res += (seq_t)a001307_g(rem - 10 * j);
        }
    }
    return res;
}
