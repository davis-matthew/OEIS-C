/*
 * A000409  Singular n X n (0,1)-matrices: the number of n X n (0,1)-matrices
 *          having distinct, nonzero ordered rows, but having at least two
 *          equal columns or at least one zero column.
 * https://oeis.org/A000409
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (-1)*Sum_{k=0..n-1} Stirling1(n+1, k+1)*binomial(2^k-1,
 *            n)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 15 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* exact binomial(2^k - 1, n) */
static seq_t a409_binom_big(idx_t k, idx_t n) {
    seq_t m = ((seq_t)1 << k) - 1;
    seq_t c = 1;
    for (idx_t i = 0; i < n; i++)
        c = c * (m - (seq_t)i) / ((seq_t)i + 1);
    return c;
}

static seq_t a409_mul(seq_t x, seq_t y) { return (seq_t)(x * 1u * y); }

/* inverse of odd q modulo 2^width (fixed-width only) */
static seq_t a409_inv(seq_t q) {
    seq_t x = q;
    for (int i = 0; i < 8; i++) x = a409_mul(x, (seq_t)(2 - a409_mul(q, x)));
    return x;
}

/* binomial(m, n) modulo 2^width, m < 2^63 */
static seq_t a409_binom_fixed(uint64_t m, idx_t n) {
    seq_t num = 1, den = 1;
    long e = 0;
    for (idx_t i = 0; i < n; i++) {
        uint64_t t = m - i;
        while (!(t & 1)) { t >>= 1; e++; }
        num = a409_mul(num, (seq_t)t);
        uint64_t d = i + 1;
        while (!(d & 1)) { d >>= 1; e--; }
        den = a409_mul(den, (seq_t)d);
    }
    seq_t r = a409_mul(num, a409_inv(den));
    if (e > 256) return 0;
    for (long i = 0; i < e; i++) r = (seq_t)(r + r);
    return r;
}

/* 1 if seq_t is a fixed-width (wrapping) type, 0 if arbitrary precision */
static int a409_fixed(void) {
    seq_t t = 1;
    for (int i = 0; i < 300; i++) t = (seq_t)(t + t);
    return t == 0;
}

seq_t a(idx_t n) {
    int fixed = a409_fixed();
    if (fixed && n > 60) return 0; /* cannot fit in any fixed width */

    /* unsigned Stirling numbers of the first kind, row n+1 */
    seq_t *s = mem_new(seq_t, (size_t)(n + 2));
    for (idx_t j = 0; j < n + 2; j++) s[j] = 0;
    s[0] = 1;
    for (idx_t i = 1; i <= n + 1; i++) {
        for (idx_t j = i; j >= 1; j--)
            s[j] = (seq_t)(s[j - 1] + a409_mul((seq_t)(i - 1), s[j]));
        s[0] = 0;
    }

    seq_t pos = 0, neg = 0;
    for (idx_t k = 0; k < n; k++) {
        if (k < 64 && ((1ULL << k) - 1) < n) continue; /* binomial is 0 */
        seq_t b = fixed ? a409_binom_fixed((1ULL << k) - 1, n)
                        : a409_binom_big(k, n);
        seq_t term = a409_mul(s[k + 1], b);
        /* sign of term is (-1)^(n-k+1) */
        if ((n - k) & 1) pos = (seq_t)(pos + term);
        else neg = (seq_t)(neg + term);
    }
    mem_free(s);
    return (seq_t)(pos - neg);
}
