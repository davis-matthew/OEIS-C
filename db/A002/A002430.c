/*
 * A002430  Numerators in Taylor series for tan(x). Also from Taylor series
 *          for tanh(x).
 * https://oeis.org/A002430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator(T(n)/(2n-1)!), where T(n) = A000182(n) are the
 *            tangent numbers, tan(x) = Sum T(n) x^(2n-1)/(2n-1)!; T(n)
 *            computed by the Brent-Harvey recurrence T[j] = (j-k)*T[j-1] +
 *            (j-k+2)*T[j], then reduced by removing common primes of (2n-1)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   a2430_bitlen
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2430_bitlen.h"

static uint32_t **a2430_T = NULL;
static size_t *a2430_L = NULL;
static idx_t a2430_N = 0;

static void a2430_build(idx_t N) {
    idx_t k, j;
    uint64_t bits = 0;
    if (a2430_T) {
        for (k = 1; k <= a2430_N; k++) free(a2430_T[k]);
        free(a2430_T);
        free(a2430_L);
    }
    a2430_T = (uint32_t **)malloc((size_t)(N + 1) * sizeof(uint32_t *));
    a2430_L = (size_t *)malloc((size_t)(N + 1) * sizeof(size_t));
    a2430_T[0] = NULL;
    a2430_L[0] = 0;
    for (k = 1; k <= N; k++) {
        if (2 * k - 2 >= 2) bits += a2430_bitlen(2 * k - 2);
        bits += a2430_bitlen(2 * k - 1);
        size_t cap = (size_t)(bits / 32) + 3;
        a2430_T[k] = (uint32_t *)calloc(cap, sizeof(uint32_t));
        a2430_L[k] = 1;
    }
    a2430_T[1][0] = 1;
    for (k = 2; k <= N; k++) {
        const uint32_t *s = a2430_T[k - 1];
        size_t sl = a2430_L[k - 1], i;
        uint32_t *d = a2430_T[k];
        uint64_t carry = 0, m = (uint64_t)(k - 1);
        for (i = 0; i < sl; i++) {
            uint64_t v = m * s[i] + carry;
            d[i] = (uint32_t)v;
            carry = v >> 32;
        }
        while (carry) { d[i++] = (uint32_t)carry; carry >>= 32; }
        a2430_L[k] = i;
    }
    for (k = 2; k <= N; k++) {
        for (j = k; j <= N; j++) {
            uint32_t *d = a2430_T[j];
            const uint32_t *s = a2430_T[j - 1];
            size_t dl = a2430_L[j], sl = a2430_L[j - 1];
            size_t m = dl > sl ? dl : sl, i;
            uint64_t ca = (uint64_t)(j - k), cb = (uint64_t)(j - k + 2), carry = 0;
            for (i = 0; i < m; i++) {
                uint64_t x = i < dl ? d[i] : 0;
                uint64_t y = i < sl ? s[i] : 0;
                uint64_t v = cb * x + ca * y + carry;
                d[i] = (uint32_t)v;
                carry = v >> 32;
            }
            while (carry) { d[i++] = (uint32_t)carry; carry >>= 32; }
            while (i > 1 && d[i - 1] == 0) i--;
            a2430_L[j] = i;
        }
    }
    a2430_N = N;
}

seq_t a(idx_t n) {
    if (n > a2430_N) {
        idx_t N = 2 * a2430_N;
        if (N < n) N = n;
        if (N < 32) N = 32;
        a2430_build(N);
    }
    size_t wl = a2430_L[n], i;
    uint32_t *w = (uint32_t *)malloc(wl * sizeof(uint32_t));
    memcpy(w, a2430_T[n], wl * sizeof(uint32_t));

    uint64_t M = 2 * (uint64_t)n - 1, p, q;
    unsigned char *comp = (unsigned char *)calloc((size_t)M + 2, 1);
    for (p = 2; p <= M; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= M; q += p) comp[q] = 1;
        uint64_t e = 0, t = M;
        while (t) { t /= p; e += t; }
        while (e > 0) {
            uint64_t r = 0;
            for (i = wl; i-- > 0;) r = ((r << 32) | w[i]) % p;
            if (r != 0) break;
            r = 0;
            for (i = wl; i-- > 0;) {
                uint64_t cur = (r << 32) | w[i];
                w[i] = (uint32_t)(cur / p);
                r = cur % p;
            }
            while (wl > 1 && w[wl - 1] == 0) wl--;
            e--;
        }
    }
    free(comp);

    seq_t res = 0;
    for (i = wl; i-- > 0;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            res = (seq_t)(res << 8);
            res += (seq_t)((w[i] >> sh) & 0xFFu);
        }
    }
    free(w);
    return res;
}
