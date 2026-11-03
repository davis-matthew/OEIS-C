/*
 * A000908  Atom-rooted polyenoids with n edges with symmetry class C_s.
 * https://oeis.org/A000908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (T(n) - 3*u(n/2) - 3*u((n+1)/2) + 3*u((n-3)/6) -
 *            u(n/3))/6 for n > 0, where T(n) = 3*binomial(2n+3,n)/(2n+3) and
 *            u(m) = binomial(2m,m)/(m+1) if m is an integer, else 0. This
 *            equals OEIS's form a(n) = A003446(n+1) - u((n-3)/6) - (u(n/3) -
 *            u((n-3)/6))/2 - (u(n/2) + (u((n+1)/2) - u((n-3)/6)))
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 26 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void z908_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void z908_div(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i > 0) {
        i--;
        uint64_t t = (r << 32) | x[i];
        x[i] = (uint32_t)(t / d);
        r = t % d;
    }
}

static void z908_mul(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void z908_set1(uint32_t *x, size_t L) {
    memset(x, 0, L * sizeof(uint32_t));
    x[0] = 1;
}

/* x = Catalan(k) */
static void z908_cat(uint32_t *x, size_t L, uint64_t k) {
    uint64_t i;
    z908_set1(x, L);
    for (i = 0; i < k; i++) {
        z908_mul(x, L, (uint32_t)(2 * (2 * i + 1)));
        z908_div(x, L, (uint32_t)(i + 2));
    }
}

static void z908_sub(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)a[i] - (uint64_t)b[i] - br;
        a[i] = (uint32_t)t;
        br = (t >> 63) & 1u;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    size_t L = (size_t)((2 * n + 3) / 32 + 4);
    uint32_t *acc = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t i;

    /* T = 3*binomial(2n+3,n)/(2n+3): ordered triples of planted Catalan branches */
    z908_set1(acc, L);
    for (i = 1; i <= n; i++) {
        z908_mul(acc, L, (uint32_t)(n + 3 + i));
        z908_div(acc, L, (uint32_t)i);
    }
    z908_mul(acc, L, 3u);
    z908_div(acc, L, (uint32_t)(2 * n + 3));

    /* + 3*s */
    if (n % 6 == 3) {
        z908_cat(tmp, L, (n - 3) / 6);
        z908_mul(tmp, L, 3u);
        z908_add(acc, tmp, L);
    }
    /* - 3*r */
    if (n % 2 == 0) z908_cat(tmp, L, n / 2);
    else z908_cat(tmp, L, (n + 1) / 2);
    z908_mul(tmp, L, 3u);
    z908_sub(acc, tmp, L);
    /* - c */
    if (n % 3 == 0) {
        z908_cat(tmp, L, n / 3);
        z908_sub(acc, tmp, L);
    }

    z908_div(acc, L, 6u);

    size_t top = L;
    while (top > 0 && acc[top - 1] == 0) top--;
    seq_t res = 0;
    while (top > 0) {
        top--;
        int k;
        for (k = 0; k < 4; k++) res = (seq_t)(res * 256u);
        res += (seq_t)acc[top];
    }
    free(acc);
    free(tmp);
    return res;
}
