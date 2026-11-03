/*
 * A002160  Nearest integer to Pi^n.
 * https://oeis.org/A002160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(Pi^n); Pi is computed exactly to enough bits with
 *            Machin's formula Pi = 16*arctan(1/5) - 4*arctan(1/239) in
 *            multi-limb fixed point, then raised to the n-th power by binary
 *            exponentiation with guard bits, and rounded to the nearest
 *            integer
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Validated: partial, pass, 29 terms through n = 28 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void p2160_add(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)a[i] + b[i];
        a[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void p2160_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i;
    for (i = L; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int p2160_iszero(const uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++)
        if (x[i]) return 0;
    return 1;
}

static void p2160_sub(uint32_t *a, const uint32_t *b, size_t L) {
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t s = (uint64_t)b[i] + br;
        uint64_t ai = a[i];
        a[i] = (uint32_t)(ai - s);
        br = (ai < s) ? 1 : 0;
    }
}

/* sum = arctan(1/x) * 2^(32W), L = W + 1 limbs */
static void p2160_atan_inv(uint32_t *sum, size_t L, size_t W, uint32_t x,
                           uint32_t *term, uint32_t *t) {
    uint32_t xx = x * x;
    uint32_t k;
    memset(term, 0, L * sizeof(uint32_t));
    term[W] = 1;
    p2160_divs(term, L, x);
    memcpy(sum, term, L * sizeof(uint32_t));
    for (k = 1;; k++) {
        p2160_divs(term, L, xx);
        if (p2160_iszero(term, L)) break;
        memcpy(t, term, L * sizeof(uint32_t));
        p2160_divs(t, L, 2 * k + 1);
        if (k & 1) p2160_sub(sum, t, L);
        else p2160_add(sum, t, L);
    }
}

/* tmp = a * b (full product) */
static void p2160_mul(const uint32_t *a, size_t la, const uint32_t *b, size_t lb, uint32_t *tmp) {
    size_t i, j;
    memset(tmp, 0, (la + lb) * sizeof(uint32_t));
    for (i = 0; i < la; i++) {
        uint64_t c = 0;
        uint64_t ai = a[i];
        if (!ai) continue;
        for (j = 0; j < lb; j++) {
            uint64_t cur = ai * b[j] + tmp[i + j] + c;
            tmp[i + j] = (uint32_t)cur;
            c = cur >> 32;
        }
        j = i + lb;
        while (c) {
            uint64_t cur = (uint64_t)tmp[j] + c;
            tmp[j] = (uint32_t)cur;
            c = cur >> 32;
            j++;
        }
    }
}

static void p2160_muls(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static uint32_t *p2160_pi = NULL;   /* Pi with p2160_pifl fraction limbs, length pifl+1 */
static size_t p2160_pifl = 0;

static void p2160_make_pi(size_t FL) {
    size_t W = FL + 2, L = W + 1, i;
    uint32_t *s1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *s2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    p2160_atan_inv(s1, L, W, 5, term, t);
    p2160_atan_inv(s2, L, W, 239, term, t);
    p2160_muls(s1, L, 4);
    p2160_sub(s1, s2, L);
    p2160_muls(s1, L, 4);
    free(p2160_pi);
    p2160_pi = (uint32_t *)calloc(FL + 1, sizeof(uint32_t));
    for (i = 0; i <= FL; i++) p2160_pi[i] = s1[i + 2];
    p2160_pifl = FL;
    free(s1); free(s2); free(term); free(t);
}

seq_t a(idx_t n) {
    uint64_t bits = (uint64_t)n * 1652 / 1000 + 2;
    size_t IL = (size_t)(bits / 32) + 2;
    size_t FL = (size_t)(bits / 32) + 4;
    size_t N = FL + IL, i;
    uint32_t *P, *R, *tmp;
    int bit, started = 0;
    seq_t r = 0;

    if (p2160_pi == NULL || p2160_pifl < FL) p2160_make_pi(FL + 16);

    P = (uint32_t *)calloc(FL + 1, sizeof(uint32_t));
    for (i = 0; i <= FL; i++) P[i] = p2160_pi[i + (p2160_pifl - FL)];

    R = (uint32_t *)calloc(N, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(2 * N + 2, sizeof(uint32_t));
    R[FL] = 1;

    for (bit = 63; bit >= 0; bit--) {
        int b = (int)((n >> bit) & 1);
        if (started) {
            p2160_mul(R, N, R, N, tmp);
            for (i = 0; i < N; i++) R[i] = tmp[i + FL];
        }
        if (b) {
            p2160_mul(R, N, P, FL + 1, tmp);
            for (i = 0; i < N; i++) R[i] = tmp[i + FL];
            started = 1;
        }
    }

    /* round: add 1/2 */
    {
        uint64_t c = (uint64_t)R[FL - 1] + 0x80000000u;
        R[FL - 1] = (uint32_t)c;
        c >>= 32;
        for (i = FL; i < N && c; i++) {
            c += R[i];
            R[i] = (uint32_t)c;
            c >>= 32;
        }
    }

    for (i = N; i-- > FL;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            idx_t byte = (idx_t)((R[i] >> sh) & 0xFFu);
            r = r * 256 + byte;
        }
    }

    free(P);
    free(R);
    free(tmp);
    return r;
}
