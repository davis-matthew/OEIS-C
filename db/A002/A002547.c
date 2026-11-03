/*
 * A002547  Numerator of the n-th harmonic number H(n) divided by (n+1); a(n)
 *          = A001008(n) / ((n+1)*A002805(n)).
 * https://oeis.org/A002547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A001008(n) / gcd(A001008(n), n+1), with H(n) = P/L where
 *            L = lcm(1..n), P = Sum_{k=1..n} L/k; a(n) = P / gcd(P, (n+1)*L),
 *            found prime-by-prime (multi-limb integers)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 24, u64 n <= 46, u128 n <= 88,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 1734 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static void b2547_add(uint32_t *x, const uint32_t *y, size_t len) {
    uint64_t c = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t v = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)v;
        c = v >> 32;
    }
}

/* q = x / d (q may alias x, or be NULL to compute only the remainder); returns x mod d */
static uint32_t b2547_div(uint32_t *q, const uint32_t *x, size_t len, uint32_t d) {
    uint64_t r = 0;
    for (size_t i = len; i-- > 0;) {
        uint64_t v = (r << 32) | x[i];
        if (q) q[i] = (uint32_t)(v / d);
        r = v % d;
    }
    return (uint32_t)r;
}

static void b2547_mul(uint32_t *x, size_t len, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t v = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)v;
        c = v >> 32;
    }
}

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    size_t len = N / 16 + 8;
    uint32_t *Lc = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(len, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(len, sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)calloc(N + 2, 1);
    size_t p, k;
    seq_t res = 0;

    Lc[0] = 1;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (k = p * 2; k <= N + 1; k += p) comp[k] = 1;
        uint64_t q = p;
        while (q <= N) { b2547_mul(Lc, len, (uint32_t)p); q *= p; }
    }
    for (k = 1; k <= N; k++) {
        b2547_div(t, Lc, len, (uint32_t)k);
        b2547_add(P, t, len);
    }
    for (p = 2; p <= N + 1; p++) {
        if (comp[p]) continue;
        unsigned e = 0;
        uint64_t q = p;
        while (q <= N) { e++; q *= p; }
        size_t m = N + 1;
        while (m % p == 0) { e++; m /= p; }
        while (e > 0) {
            if (b2547_div(NULL, P, len, (uint32_t)p) != 0) break;
            b2547_div(P, P, len, (uint32_t)p);
            e--;
        }
    }
    for (size_t i = len; i-- > 0;) {
        for (int s = 24; s >= 0; s -= 8) {
            res = (seq_t)(res * 256);
            res = (seq_t)(res + (int)((P[i] >> s) & 255u));
        }
    }
    free(Lc); free(P); free(t); free(comp);
    return res;
}
