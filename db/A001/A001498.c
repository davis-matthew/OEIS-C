/*
 * A001498  Triangle a(n,k) (n >= 0, 0 <= k <= n) of coefficients of Bessel
 *          polynomials y_n(x) (exponents in increasing order).
 * https://oeis.org/A001498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   a(n) = T(r,k) with n = r*(r+1)/2 + k, 0 <= k <= r, and T(r,k) =
 *            (r+k)!/(2^k*(r-k)!*k!), evaluated as a product of prime powers
 *            using Legendre's formula for factorial valuations
 * Range:     u8 n <= 17, u16 n <= 33, u32 n <= 74, u64 n <= 185,
 *            u128 n <= 462, big any n
 * Validated: standard, pass, 71 terms through n = 1277 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/is_prime_u64.h"

static uint64_t b1498_legendre(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t idx) {
    idx_t n = 0, start = 0, k, p;
    seq_t r = 1;

    while (start + n + 1 <= idx) {
        start += n + 1;
        n++;
    }
    k = idx - start;

    for (p = 2; p <= n + k; p++) {
        uint64_t e, i;
        if (!is_prime_u64(p)) continue;
        e = b1498_legendre(n + k, p) - b1498_legendre(n - k, p) - b1498_legendre(k, p);
        if (p == 2) e -= k;
        for (i = 0; i < e; i++) r *= (seq_t)p;
    }
    return r;
}
