/*
 * A003236  a(n) = Sum_{k=0..n} (-1)^(n-k) C(n,k)*C((k+1)^2, n).
 * https://oeis.org/A003236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) C(n,k)*C((k+1)^2, n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 292 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t a3236_inv(seq_t q) {
    /* inverse of odd q modulo 2^w (Newton iteration) */
    seq_t x = q;
    int i;
    for (i = 0; i < 8; i++) x = x * (2 - q * x);
    return x;
}

/* C(m, r) modulo 2^w, fixed-width build */
static seq_t a3236_binom_mod(idx_t m, idx_t r, int w) {
    if (r > m) return 0;
    if (r > m - r) r = m - r;
    seq_t num = 1, den = 1;
    long long e = 0;
    idx_t i;
    for (i = 1; i <= r; i++) {
        uint64_t t = (uint64_t)(m - r + i);
        uint64_t d = (uint64_t)i;
        while ((t & 1) == 0) { t >>= 1; e++; }
        while ((d & 1) == 0) { d >>= 1; e--; }
        num = num * (seq_t)t;
        den = den * (seq_t)d;
    }
    if (e >= w) return 0;
    seq_t res = num * a3236_inv(den);
    long long j;
    for (j = 0; j < e; j++) res = res * 2;
    return res;
}

static int a3236_width(void) {
    seq_t x = 1;
    int w = 0;
    while (w < 1024) {
        x = x * 2;
        w++;
        if (x == 0) return w;
    }
    return 0; /* arbitrary precision */
}

seq_t a(idx_t n) {
    int w = a3236_width();
    seq_t pos = 0, neg = 0;
    idx_t k;
    for (k = 0; k <= n; k++) {
        idx_t m = (k + 1) * (k + 1);
        if (m < n) continue;
        seq_t term;
        if (w) term = a3236_binom_mod(n, k, w) * a3236_binom_mod(m, n, w);
        else term = binomial(n, k) * binomial(m, n);
        if (((n - k) & 1) == 0) pos += term;
        else neg += term;
    }
    return pos - neg;
}
