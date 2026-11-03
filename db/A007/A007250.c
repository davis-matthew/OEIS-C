/*
 * A007250  McKay-Thompson series of class 4a for the Monster group.
 * https://oeis.org/A007250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = A007249(n) - 64*A022577(n-1), where A007249 =
 *            (eta(q)/eta(q^2))^12 = Product_{k odd} (1-q^k)^12 = A(q),
 *            A022577 = (eta(q^2)/eta(q))^12 = Product_{k odd} (1-q^k)^(-12) =
 *            B(q) = 1/A(q). Big build: n*A(n) = -12*Sum_{k=1..n}
 *            sigma_odd(k)*A(n-k), n*B(n) = 12*Sum sigma_odd(k)*B(n-k). Fixed
 *            width...
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 15, i64 n <= 57, i128 n <= 218,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 996 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *zalloc_arr(idx_t len) {
    return mem_new(seq_t, (size_t)len);
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

typedef unsigned __int128 u128_t;

static seq_t *res_cache = NULL;
static idx_t res_cap = 0;

static void mul_u128(const u128_t *x, const u128_t *y, u128_t *out, idx_t L) {
    for (idx_t i = 0; i < L; i++) out[i] = 0;
    for (idx_t i = 0; i < L; i++) {
        if (x[i] == 0) continue;
        for (idx_t j = 0; i + j < L; j++) out[i + j] += x[i] * y[j];
    }
}

static void build_fixed(seq_t *res, idx_t L) {
    u128_t *P = (u128_t *)calloc(L, sizeof(u128_t));
    u128_t *P2 = (u128_t *)calloc(L, sizeof(u128_t));
    u128_t *P4 = (u128_t *)calloc(L, sizeof(u128_t));
    u128_t *P8 = (u128_t *)calloc(L, sizeof(u128_t));
    u128_t *A = (u128_t *)calloc(L, sizeof(u128_t));
    u128_t *B = (u128_t *)calloc(L, sizeof(u128_t));
    P[0] = 1;
    for (idx_t k = 1; k < L; k += 2)
        for (idx_t j = L - 1; j >= k; j--) P[j] -= P[j - k];
    mul_u128(P, P, P2, L);
    mul_u128(P2, P2, P4, L);
    mul_u128(P4, P4, P8, L);
    mul_u128(P8, P4, A, L);
    B[0] = 1;
    for (idx_t i = 1; i < L; i++) {
        u128_t t = 0;
        for (idx_t j = 1; j <= i; j++) t += A[j] * B[i - j];
        B[i] = (u128_t)0 - t;
    }
    for (idx_t i = 0; i < L; i++) {
        u128_t v = A[i];
        if (i > 0) v -= (u128_t)64 * B[i - 1];
        res[i] = (seq_t)(__int128)v;
    }
    free(P); free(P2); free(P4); free(P8); free(A); free(B);
}

static void build_big(seq_t *res, idx_t L) {
    long long *s = (long long *)calloc(L, sizeof(long long));
    for (idx_t d = 1; d < L; d += 2)
        for (idx_t m = d; m < L; m += d) s[m] += (long long)d;
    seq_t *A = zalloc_arr(L);
    seq_t *B = zalloc_arr(L);
    A[0] = 1;
    B[0] = 1;
    for (idx_t m = 1; m < L; m++) {
        seq_t ta = 0, tb = 0;
        for (idx_t k = 1; k <= m; k++) {
            ta += A[m - k] * s[k];
            tb += B[m - k] * s[k];
        }
        A[m] = (ta * (-12)) / (seq_t)m;
        B[m] = (tb * 12) / (seq_t)m;
    }
    for (idx_t i = 0; i < L; i++) {
        seq_t v = A[i];
        if (i > 0) v -= B[i - 1] * 64;
        res[i] = v;
    }
    release(A, L);
    release(B, L);
    free(s);
}

seq_t a(idx_t n) {
    if (n >= res_cap) {
        idx_t L = res_cap * 2;
        if (L < n + 1) L = n + 1;
        if (L < 64) L = 64;
        seq_t *r = zalloc_arr(L);
        if (seq_is_big()) build_big(r, L);
        else build_fixed(r, L);
        if (res_cache) release(res_cache, res_cap);
        res_cache = r;
        res_cap = L;
    }
    return res_cache[n];
}
