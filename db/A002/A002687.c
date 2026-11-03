/*
 * A002687  Numerators of coefficients for repeated integration.
 * https://oeis.org/A002687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator((1/n!) * Sum_{k=1..n}
 *            Stirling1(n,k)/((k+1)*(k+2))), computed exactly with D =
 *            lcm(1..n+2): N = Sum Stirling1(n,k)*D/((k+1)(k+2)), value =
 *            N/(n!*D), reduced by removing the small primes of n!*D from N
 * Range:     i8 n <= 5, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 34,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 425 (2026-11-03)
 * Helpers:   bit_length_u64, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bit_length_u64.h"
#include "../utils/is_prime_u64.h"

static void z2687_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static int z2687_cmp(const uint32_t *a, const uint32_t *b, size_t L) {
    for (size_t i = L; i-- > 0;) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void z2687_divs(uint32_t *x, size_t L, uint64_t d) {
    uint64_t r = 0;
    for (size_t i = L; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static size_t z2687_len(const uint32_t *x, size_t L) {
    while (L > 0 && x[L - 1] == 0) L--;
    return L;
}

static uint64_t z2687_mods(const uint32_t *x, size_t L, uint64_t d) {
    uint64_t r = 0;
    for (size_t i = L; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        r = cur % d;
    }
    return r;
}

static void z2687_muladd(uint32_t *acc, const uint32_t *a, const uint32_t *b, size_t L) {
    size_t la = z2687_len(a, L), lb = z2687_len(b, L);
    for (size_t i = 0; i < la; i++) {
        uint64_t carry = 0;
        size_t j;
        for (j = 0; j < lb && i + j < L; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + acc[i + j] + carry;
            acc[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        size_t k = i + j;
        while (carry && k < L) {
            uint64_t t = (uint64_t)acc[k] + carry;
            acc[k] = (uint32_t)t;
            carry = t >> 32;
            k++;
        }
    }
}

static void z2687_muls(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void z2687_sub(uint32_t *a, const uint32_t *b, size_t L) {
    int64_t br = 0;
    for (size_t i = 0; i < L; i++) {
        int64_t t = (int64_t)a[i] - (int64_t)b[i] - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)t;
    }
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 2;
    uint64_t fb = 0;
    for (uint64_t i = 2; i <= n; i++) fb += (uint64_t)bit_length_u64(i);
    size_t L = (size_t)((fb + 2 * m + 64) / 32 + 2);

    uint32_t *c = (uint32_t *)calloc((size_t)(n + 1) * L, sizeof(uint32_t));
    uint32_t *D = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *w = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *M = (uint32_t *)calloc(L, sizeof(uint32_t));
    if (!c || !D || !w || !P || !M) {
        free(c); free(D); free(w); free(P); free(M);
        return 0;
    }

    /* unsigned Stirling numbers of the first kind, row n */
    c[0] = 1;
    uint64_t rb = 0;
    for (uint64_t r = 1; r <= n; r++) {
        rb += (uint64_t)bit_length_u64(r);
        size_t lim = (size_t)(rb / 32 + 2);
        if (lim > L) lim = L;
        for (uint64_t k = r; k >= 1; k--) {
            uint32_t *X = c + (size_t)k * L;
            z2687_muls(X, lim, (uint32_t)(r - 1));
            z2687_add(X, c + (size_t)(k - 1) * L, lim);
        }
        memset(c, 0, L * sizeof(uint32_t));
    }

    /* D = lcm(1..n+2) */
    D[0] = 1;
    for (uint64_t p = 2; p <= m; p++) {
        if (!is_prime_u64(p)) continue;
        uint64_t q = p;
        z2687_muls(D, L, (uint32_t)p);
        while (q <= m / p) { q *= p; z2687_muls(D, L, (uint32_t)p); }
    }

    for (uint64_t k = 1; k <= n; k++) {
        memcpy(w, D, L * sizeof(uint32_t));
        z2687_divs(w, L, (k + 1) * (k + 2));
        z2687_muladd(((n - k) & 1) ? M : P, c + (size_t)k * L, w, L);
    }

    int neg = 0;
    uint32_t *N;
    if (z2687_cmp(P, M, L) < 0) { z2687_sub(M, P, L); N = M; neg = 1; }
    else { z2687_sub(P, M, L); N = P; }

    /* cancel common factors with the denominator n! * D */
    for (uint64_t p = 2; p <= m; p++) {
        if (!is_prime_u64(p)) continue;
        uint64_t e = 0;
        for (uint64_t t = n / p; t > 0; t /= p) e += t;
        for (uint64_t q = p; q <= m; ) { e++; if (q > m / p) break; q *= p; }
        while (e > 0 && z2687_mods(N, L, p) == 0) {
            z2687_divs(N, L, p);
            e--;
        }
    }

    size_t ln = z2687_len(N, L);
    seq_t result = 0;
    for (size_t i = ln; i-- > 0;) {
        result = result * 65536 + (seq_t)(idx_t)(N[i] >> 16);
        result = result * 65536 + (seq_t)(idx_t)(N[i] & 0xFFFFu);
    }
    if (neg) result = -result;

    free(c); free(D); free(w); free(P); free(M);
    return result;
}
