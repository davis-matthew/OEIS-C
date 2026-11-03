/*
 * A002206  Numerators of logarithmic numbers (also of Gregory coefficients
 *          G(n)).
 * https://oeis.org/A002206
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign, frac, nice
 * Formula:   a(n) = numerator(G(n+1)), where G(m) = (1/m!) * Sum_{k=0..m}
 *            Stirling1(m,k)/(k+1) (Gregory coefficients, 1/log(1+x) =
 *            Sum_{n>=-1} G(n+1) x^n). Computed exactly with multi-limb
 *            integers: T = Sum_k Stirling1(m,k) * (L/(k+1)) with L =
 *            lcm(1..m+1), so G(m) = T/(L*m!); T is then reduced by the
 *            primes...
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 97 (2026-11-01)
 * Helpers:   smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/smallest_prime_factor.h"

static size_t g2206_trim(const uint32_t *x, size_t n) {
    while (n > 0 && x[n - 1] == 0) n--;
    return n;
}

static void g2206_add(uint32_t *acc, size_t *al, const uint32_t *x, size_t xl) {
    size_t n = (*al > xl) ? *al : xl;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t t = carry;
        if (i < *al) t += acc[i];
        if (i < xl) t += x[i];
        acc[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) acc[n++] = (uint32_t)carry;
    *al = g2206_trim(acc, n);
}

static int g2206_cmp(const uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
    if (xl != yl) return (xl > yl) ? 1 : -1;
    while (xl > 0) {
        xl--;
        if (x[xl] != y[xl]) return (x[xl] > y[xl]) ? 1 : -1;
    }
    return 0;
}

static uint32_t g2206_divsmall(uint32_t *x, size_t *len, uint32_t d) {
    uint64_t r = 0;
    size_t i = *len;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    *len = g2206_trim(x, *len);
    return (uint32_t)r;
}

/* dst = dst*m + src */
static void g2206_fma(uint32_t *dst, size_t *dl, const uint32_t *src, size_t sl, uint32_t m) {
    size_t n = (*dl > sl) ? *dl : sl;
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t t = carry;
        if (i < *dl) t += (uint64_t)dst[i] * m;
        if (i < sl) t += src[i];
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
    if (carry) dst[n++] = (uint32_t)carry;
    *dl = g2206_trim(dst, n);
}

static uint32_t g2206_modsmall(const uint32_t *x, size_t len, uint32_t d) {
    uint64_t r = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        r = ((r << 32) | x[i]) % d;
    }
    return (uint32_t)r;
}

static size_t g2206_mul(uint32_t *out, const uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
    size_t i, j;
    memset(out, 0, (xl + yl + 1) * sizeof(uint32_t));
    for (i = 0; i < xl; i++) {
        uint64_t carry = 0;
        for (j = 0; j < yl; j++) {
            uint64_t t = (uint64_t)x[i] * y[j] + out[i + j] + carry;
            out[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        size_t k = i + yl;
        while (carry) {
            uint64_t t = (uint64_t)out[k] + carry;
            out[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
    return g2206_trim(out, xl + yl + 1);
}

/* x -= y, requires x >= y */
static void g2206_sub(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < *xl; i++) {
        int64_t t = (int64_t)x[i] - borrow - (i < yl ? (int64_t)y[i] : 0);
        if (t < 0) { t += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        x[i] = (uint32_t)t;
    }
    *xl = g2206_trim(x, *xl);
}

seq_t a(idx_t n) {
    if (n < -1) return 0;
    size_t m = (size_t)(n + 1);
    size_t bl = 0, t = m + 1;
    while (t) { bl++; t >>= 1; }
    size_t cap = (m * bl) / 32 + 3;
    size_t Lcap = (2 * (m + 2)) / 32 + 3;
    size_t acap = cap + Lcap + 3;
    size_t j, k;

    uint32_t *S = (uint32_t *)calloc((m + 1) * cap, sizeof(uint32_t));
    size_t *len = (size_t *)calloc(m + 1, sizeof(size_t));
    S[0] = 1;
    len[0] = 1;
    /* unsigned Stirling numbers of the first kind: c(j+1,k) = c(j,k-1) + j*c(j,k) */
    for (j = 0; j < m; j++) {
        for (k = j + 1; k >= 1; k--)
            g2206_fma(S + k * cap, &len[k], S + (k - 1) * cap, len[k - 1], (uint32_t)j);
        len[0] = 0;
    }

    uint32_t *L = (uint32_t *)calloc(Lcap, sizeof(uint32_t));
    size_t Ll = 1;
    L[0] = 1;
    for (j = 2; j <= m + 1; j++) {
        size_t p = smallest_prime_factor(j), q = j;
        while (q % p == 0) q /= p;
        if (q == 1) g2206_fma(L, &Ll, NULL, 0, (uint32_t)p);
    }

    uint32_t *Q = (uint32_t *)calloc(Lcap, sizeof(uint32_t));
    uint32_t *prod = (uint32_t *)calloc(acap, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(acap, sizeof(uint32_t));
    uint32_t *N = (uint32_t *)calloc(acap, sizeof(uint32_t));
    size_t Pl = 0, Nl = 0;

    for (k = 0; k <= m; k++) {
        if (len[k] == 0) continue;
        size_t Ql = Ll;
        memcpy(Q, L, Ll * sizeof(uint32_t));
        g2206_divsmall(Q, &Ql, (uint32_t)(k + 1));
        size_t pl = g2206_mul(prod, S + k * cap, len[k], Q, Ql);
        if ((m - k) & 1) g2206_add(N, &Nl, prod, pl);
        else g2206_add(P, &Pl, prod, pl);
    }

    int neg = 0;
    uint32_t *T;
    size_t Tl;
    if (g2206_cmp(P, Pl, N, Nl) >= 0) {
        g2206_sub(P, &Pl, N, Nl);
        T = P; Tl = Pl;
    } else {
        g2206_sub(N, &Nl, P, Pl);
        T = N; Tl = Nl; neg = 1;
    }

    /* reduce by common factors with denominator L * m! */
    for (j = 2; j <= m + 1 && Tl > 0; j++) {
        if (smallest_prime_factor(j) != j) continue;
        size_t p = j, e = 0, q;
        for (q = m; q > 0; q /= p) e += q / p;
        for (q = p; q <= m + 1; q *= p) e++;
        while (e > 0 && g2206_modsmall(T, Tl, (uint32_t)p) == 0) {
            g2206_divsmall(T, &Tl, (uint32_t)p);
            e--;
        }
    }

    seq_t v = 0;
    size_t i = Tl;
    while (i > 0) {
        i--;
        int b;
        for (b = 3; b >= 0; b--)
            v = v * 256 + (seq_t)((T[i] >> (8 * b)) & 255u);
    }
    if (neg) v = -v;

    free(S); free(len); free(L); free(Q); free(prod); free(P); free(N);
    return v;
}
