/*
 * A001880  Coefficients of Bessel polynomials y_n (x).
 * https://oeis.org/A001880
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   (n-4)*a(n) = (n-2)*(2*n-5)*a(n-1) for n >= 5, a(4) = 1;
 *            equivalently a(n) = (2*n-4)!/(4!*(n-4)!*2^(n-4))
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 98 (2026-11-01)
 * Helpers:   gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd_idx.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    if (n < 4) return 0;
    for (k = 5; k <= n; k++) {
        idx_t d = k - 4;
        idx_t p = k - 2;
        idx_t q = 2 * k - 5;
        idx_t g = gcd_idx(p, d);
        p /= g;
        d /= g;
        g = gcd_idx(q, d);
        q /= g;
        d /= g;
        /* d now divides r exactly, since d is coprime to p*q */
        r /= (seq_t)d;
        r *= (seq_t)p;
        r *= (seq_t)q;
    }
    return r;
}
