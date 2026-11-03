/*
 * A004717  Positions of 1's in the binary expansion of log(4)/log(3)-1.
 * https://oeis.org/A004717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1-bit in the binary expansion of x =
 *            log(4)/log(3) - 1 = log(4/3)/log(3) =
 *            atanh(1/7)/(atanh(1/3)+atanh(1/5)). x is computed exactly to P
 *            bits with big fixed-point integers (atanh series plus long
 *            division), then the bits are scanned
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a004717_add(uint32_t *a, const uint32_t *b, int L) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void a004717_divsmall(uint32_t *a, int L, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | a[i];
        a[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int a004717_iszero(const uint32_t *a, int L) {
    int i;
    for (i = 0; i < L; i++) if (a[i]) return 0;
    return 1;
}

/* out += 2^W * atanh(1/k) */
static void a004717_atanh(uint32_t *out, int L, long long W, uint32_t k) {
    uint32_t *p = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t j;
    p[W / 32] = 1u << (W % 32);
    a004717_divsmall(p, L, k);
    for (j = 0; ; j++) {
        memcpy(t, p, (size_t)L * sizeof(uint32_t));
        a004717_divsmall(t, L, 2 * j + 1);
        a004717_add(out, t, L);
        a004717_divsmall(p, L, k * k);
        if (a004717_iszero(p, L)) break;
    }
    free(p);
    free(t);
}

static int a004717_geq(const uint32_t *a, const uint32_t *b, int L) {
    int i;
    for (i = L - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] > b[i];
    }
    return 1;
}

static void a004717_shl1(uint32_t *a, int L) {
    uint32_t c = 0;
    int i;
    for (i = 0; i < L; i++) {
        uint32_t nc = a[i] >> 31;
        a[i] = (a[i] << 1) | c;
        c = nc;
    }
}

static void a004717_sub(uint32_t *a, const uint32_t *b, int L) {
    int64_t br = 0;
    int i;
    for (i = 0; i < L; i++) {
        int64_t s = (int64_t)a[i] - b[i] - br;
        if (s < 0) { s += ((int64_t)1 << 32); br = 1; } else br = 0;
        a[i] = (uint32_t)s;
    }
}

static long long *a004717_pos = NULL;
static long long a004717_cnt = 0;
static long long a004717_P = 0;

static void a004717_compute(long long P) {
    long long W = P + 96;
    int L = (int)(W / 32) + 3;
    uint32_t *A = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    long long i, cap;
    a004717_atanh(A, L, W, 7);
    a004717_atanh(B, L, W, 3);
    a004717_atanh(B, L, W, 5);
    free(a004717_pos);
    cap = P + 1;
    a004717_pos = (long long *)malloc((size_t)cap * sizeof(long long));
    a004717_cnt = 0;
    for (i = 1; i <= P; i++) {
        a004717_shl1(A, L);
        if (a004717_geq(A, B, L)) {
            a004717_sub(A, B, L);
            a004717_pos[a004717_cnt++] = i;
        }
    }
    a004717_P = P;
    free(A);
    free(B);
}

seq_t a(idx_t n) {
    if ((long long)n > a004717_cnt) {
        long long P = a004717_P ? a004717_P : 0;
        if (P < 2 * (long long)n + 256) P = 2 * (long long)n + 256;
        for (;;) {
            a004717_compute(P);
            if (a004717_cnt >= (long long)n) break;
            P *= 2;
        }
    }
    return (seq_t)a004717_pos[n - 1];
}
