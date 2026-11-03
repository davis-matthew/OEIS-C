/*
 * A000846  a(n) = C(3n,n) - C(2n,n).
 * https://oeis.org/A000846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(3n,n) - C(2n,n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 196 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* C(m,k) mod 2^w, for fixed-width seq_t */
static seq_t binom_fixed(idx_t m, idx_t k, int w) {
    seq_t num = 1, den = 1, x, r;
    long long e = 0;
    idx_t i;
    int t;
    for (i = 1; i <= k; i++) {
        uint64_t p = m - k + i, q = i;
        while (!(p & 1)) { p >>= 1; e++; }
        while (!(q & 1)) { q >>= 1; e--; }
        num = (seq_t)(1u * num * (seq_t)p);
        den = (seq_t)(1u * den * (seq_t)q);
    }
    if (e >= w) return 0;
    x = den;
    for (t = 0; t < 8; t++)
        x = (seq_t)(1u * x * (seq_t)(2u - 1u * den * x));
    r = (seq_t)(1u * num * x);
    while (e-- > 0) r = (seq_t)(r + r);
    return r;
}

static int seq_width(void) {
    static int w = -1;
    if (w < 0) {
        seq_t x = 1;
        int c = 0;
        while (x != 0 && c < 300) {
            x = (seq_t)(x + x);
            c++;
        }
        w = c;
    }
    return w;
}

seq_t a(idx_t n) {
    int w = seq_width();
    if (w >= 300)
        return binomial(3 * n, n) - binomial(2 * n, n);
    return (seq_t)(binom_fixed(3 * n, n, w) - binom_fixed(2 * n, n, w));
}
