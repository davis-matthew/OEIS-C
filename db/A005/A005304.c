/*
 * A005304  Representation degeneracies for Ramond strings.
 * https://oeis.org/A005304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = [x^(n-1)] 2 * Product_{k>=1} (1 - x^k)^(-c(k)), c(k) = 1,
 *            1, 3, 3, 4, 3, then c(k) = 4 for odd k, 2 for even k (k >= 7).
 *            Equivalently G.f. = 2 * (1-x)^3 (1-x^2) (1-x^3) /
 *            ((1-x^4)(1-x^6)) / T(x)^2, with T(x) = Sum_{m=-inf..inf} (-1)^m
 *            x^(m^2) = Product (1-x^k)/(1+x^k)
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *B = 0;
static idx_t Blen = 0;

static seq_t *build(idx_t len) {
    seq_t *g = alloc_zero(len);
    seq_t *h = alloc_zero(len);
    seq_t *b = alloc_zero(len);
    idx_t i, m;

    /* g = 1/T = Product (1+x^k)/(1-x^k) */
    for (i = 0; i < len; i++) {
        if (i == 0) { g[0] = 1; continue; }
        seq_t pos = 0, neg = 0;
        for (m = 1; m * m <= i; m++) {
            if (m & 1) pos += g[i - m * m];
            else neg += g[i - m * m];
        }
        seq_t t = pos;
        t -= neg;
        g[i] = t + t;
    }
    /* h = g/T = Product ((1+x^k)/(1-x^k))^2 */
    for (i = 0; i < len; i++) {
        seq_t pos = 0, neg = 0;
        for (m = 1; m * m <= i; m++) {
            if (m & 1) pos += h[i - m * m];
            else neg += h[i - m * m];
        }
        seq_t t = g[i];
        t += pos;
        t += pos;
        t -= neg;
        t -= neg;
        h[i] = t;
    }
    for (i = 0; i < len; i++) b[i] = h[i];

    /* multiply by (1-x)^3 (1-x^2) (1-x^3); every intermediate stays nonnegative */
    int r;
    for (r = 0; r < 3; r++)
        for (i = len - 1; i >= 1; i--) b[i] -= b[i - 1];
    for (i = len - 1; i >= 2; i--) b[i] -= b[i - 2];
    for (i = len - 1; i >= 3; i--) b[i] -= b[i - 3];
    /* divide by (1-x^4)(1-x^6) */
    for (i = 4; i < len; i++) b[i] += b[i - 4];
    for (i = 6; i < len; i++) b[i] += b[i - 6];

    release(g, len);
    release(h, len);
    return b;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (Blen < n) {
        idx_t newlen = Blen * 2;
        if (newlen < n) newlen = n;
        if (newlen < 64) newlen = 64;
        seq_t *nb = build(newlen);
        if (B) release(B, Blen);
        B = nb;
        Blen = newlen;
    }
    seq_t r = B[n - 1];
    return r + r;
}
