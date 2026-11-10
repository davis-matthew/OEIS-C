/*
 * A087746  Erroneous version of A023036.
 * https://oeis.org/A087746
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(1) = 5 (the erroneous term); a(n) = A001172(n) for n > 1,
 *            where A001172(n) is the smallest even number that is an
 *            unordered sum of two odd primes in exactly n ways (found by an
 *            NTT-based sieve search)
 * Range:     i8 n <= 10, i16 n <= 53, i32 n <= 53, i64 n <= 53, i128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define A1172_MOD 998244353ULL
#define A1172_G 3ULL
#define A1172_MAXM (1ULL << 22)

static uint32_t *a1172_first = NULL;
static uint64_t a1172_M = 0;

static uint64_t a1172_pw(uint64_t b, uint64_t e) {
    uint64_t r = 1;
    b %= A1172_MOD;
    while (e) {
        if (e & 1) r = r * b % A1172_MOD;
        b = b * b % A1172_MOD;
        e >>= 1;
    }
    return r;
}

static void a1172_ntt(uint32_t *x, uint64_t len, int inv) {
    uint64_t i, j, k, half, step;
    for (i = 1, j = 0; i < len; i++) {
        uint64_t bit = len >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = x[i]; x[i] = x[j]; x[j] = t; }
    }
    for (step = 2; step <= len; step <<= 1) {
        uint64_t w = a1172_pw(A1172_G, (A1172_MOD - 1) / step);
        if (inv) w = a1172_pw(w, A1172_MOD - 2);
        half = step >> 1;
        uint32_t *tw = (uint32_t *)malloc(sizeof(uint32_t) * half);
        uint64_t cur = 1;
        for (k = 0; k < half; k++) { tw[k] = (uint32_t)cur; cur = cur * w % A1172_MOD; }
        for (i = 0; i < len; i += step) {
            for (k = 0; k < half; k++) {
                uint64_t u = x[i + k];
                uint64_t v = (uint64_t)x[i + k + half] * tw[k] % A1172_MOD;
                uint64_t s = u + v;
                if (s >= A1172_MOD) s -= A1172_MOD;
                x[i + k] = (uint32_t)s;
                x[i + k + half] = (uint32_t)(u + A1172_MOD - v >= A1172_MOD ? u - v : u + A1172_MOD - v);
            }
        }
        free(tw);
    }
    if (inv) {
        uint64_t il = a1172_pw(len, A1172_MOD - 2);
        for (i = 0; i < len; i++) x[i] = (uint32_t)((uint64_t)x[i] * il % A1172_MOD);
    }
}

static void a1172_build(uint64_t M) {
    uint64_t lim = 2 * M + 1, i, j, s;
    unsigned char *comp = (unsigned char *)calloc(lim + 1, 1);
    for (i = 3; i * i <= lim; i += 2)
        if (!comp[i])
            for (j = i * i; j <= lim; j += 2 * i) comp[j] = 1;
    unsigned char *g = (unsigned char *)calloc(M, 1);
    for (i = 1; i < M; i++) g[i] = !comp[2 * i + 1];
    free(comp);

    uint64_t len = 2 * M;
    uint32_t *h = (uint32_t *)calloc(len, sizeof(uint32_t));
    for (i = 0; i < M; i++) h[i] = g[i];
    a1172_ntt(h, len, 0);
    for (i = 0; i < len; i++) h[i] = (uint32_t)((uint64_t)h[i] * h[i] % A1172_MOD);
    a1172_ntt(h, len, 1);

    uint32_t *first = (uint32_t *)calloc(M + 1, sizeof(uint32_t));
    for (s = 1; s < M; s++) {
        uint64_t c = h[s];
        if ((s & 1) == 0 && g[s / 2]) c++;
        c /= 2;
        if (c <= M && first[c] == 0) first[c] = (uint32_t)(2 * s + 2);
    }
    free(h);
    free(g);
    free(a1172_first);
    a1172_first = first;
    a1172_M = M;
}

static seq_t A001172_a(idx_t n) {
    if (n == 0) return 0;
    for (;;) {
        if (a1172_first != NULL && (uint64_t)n <= a1172_M && a1172_first[n] != 0)
            return (seq_t)a1172_first[n];
        uint64_t M = a1172_M ? a1172_M * 2 : (1ULL << 15);
        if (M > A1172_MAXM) return 0;
        a1172_build(M);
    }
}

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)5;
    return A001172_a(n);
}
