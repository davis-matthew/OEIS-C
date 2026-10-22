/*
 * A002949  Continued fraction for cube root of 6.
 * https://oeis.org/A002949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction of 6^(1/3), computed exactly by Lagrange's
 *            method. Start with P(x) = x^3 - 6. Repeatedly let a(n) be the
 *            floor of P's unique real root (found by sign tests), then
 *            replace P(x) by x^3 * P(a(n) + 1/x). All arithmetic uses exact
 *            multiprecision integers
 * Range:     u8 n <= 5, u16 n <= 3242, u32 n <= 9325, u64 n <= 9325,
 *            u128 n <= 9325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *A2949_C[4];
static uint32_t *A2949_tmp;
static size_t A2949_W = 0;
static uint32_t *A2949_T = NULL;
static idx_t A2949_cnt = 0, A2949_cap = 0;

static int a2949_neg(const uint32_t *x) { return (int)(x[A2949_W - 1] >> 31); }

/* dst = src*m + add (two's complement, modulo 2^(32W)); aliasing at same index is fine */
static void a2949_muladd(uint32_t *dst, const uint32_t *src, uint32_t m, const uint32_t *add) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < A2949_W; i++) {
        uint64_t t = (uint64_t)src[i] * m + add[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void a2949_ensure(void) {
    int need = 0, j;
    size_t i;
    for (j = 0; j < 4 && !need; j++) {
        uint32_t e = a2949_neg(A2949_C[j]) ? 0xFFFFFFFFu : 0u;
        for (i = A2949_W - 6; i < A2949_W; i++)
            if (A2949_C[j][i] != e) { need = 1; break; }
    }
    if (!need) return;
    {
        size_t nw = A2949_W * 2;
        for (j = 0; j < 4; j++) {
            uint32_t e = a2949_neg(A2949_C[j]) ? 0xFFFFFFFFu : 0u;
            uint32_t *p = (uint32_t *)malloc(nw * sizeof(uint32_t));
            memcpy(p, A2949_C[j], A2949_W * sizeof(uint32_t));
            for (i = A2949_W; i < nw; i++) p[i] = e;
            free(A2949_C[j]);
            A2949_C[j] = p;
        }
        free(A2949_tmp);
        A2949_tmp = (uint32_t *)malloc(nw * sizeof(uint32_t));
        A2949_W = nw;
    }
}

/* returns 1 if P(m) has sign opposite to the leading coefficient (m below root) */
static int a2949_below(uint32_t m) {
    memcpy(A2949_tmp, A2949_C[0], A2949_W * sizeof(uint32_t));
    a2949_muladd(A2949_tmp, A2949_tmp, m, A2949_C[1]);
    a2949_muladd(A2949_tmp, A2949_tmp, m, A2949_C[2]);
    a2949_muladd(A2949_tmp, A2949_tmp, m, A2949_C[3]);
    return a2949_neg(A2949_tmp) != a2949_neg(A2949_C[0]);
}

static void a2949_init(void) {
    int j;
    size_t i;
    A2949_W = 8;
    for (j = 0; j < 4; j++) A2949_C[j] = (uint32_t *)calloc(A2949_W, sizeof(uint32_t));
    A2949_tmp = (uint32_t *)calloc(A2949_W, sizeof(uint32_t));
    A2949_C[0][0] = 1;
    for (i = 0; i < A2949_W; i++) A2949_C[3][i] = 0xFFFFFFFFu;
    A2949_C[3][0] = (uint32_t)(0u - 6u);
}

static void a2949_step(void) {
    uint64_t lo = 1, hi = 2, k;
    int i, j;
    uint32_t *t;
    a2949_ensure();
    while (hi < 0x80000000ull && a2949_below((uint32_t)hi)) { lo = hi; hi *= 2; }
    if (hi >= 0x80000000ull && a2949_below((uint32_t)hi)) { lo = hi; hi = 0xFFFFFFFFull; }
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (a2949_below((uint32_t)mid)) lo = mid; else hi = mid;
    }
    k = lo;
    for (i = 0; i < 3; i++)
        for (j = 1; j <= 3 - i; j++)
            a2949_muladd(A2949_C[j], A2949_C[j - 1], (uint32_t)k, A2949_C[j]);
    t = A2949_C[0]; A2949_C[0] = A2949_C[3]; A2949_C[3] = t;
    t = A2949_C[1]; A2949_C[1] = A2949_C[2]; A2949_C[2] = t;
    if (A2949_cnt == A2949_cap) {
        idx_t nc = A2949_cap ? A2949_cap * 2 : 64;
        uint32_t *p = (uint32_t *)malloc(nc * sizeof(uint32_t));
        if (A2949_cnt) memcpy(p, A2949_T, A2949_cnt * sizeof(uint32_t));
        free(A2949_T);
        A2949_T = p;
        A2949_cap = nc;
    }
    A2949_T[A2949_cnt++] = (uint32_t)k;
}

seq_t a(idx_t n) {
    if (A2949_W == 0) a2949_init();
    while (A2949_cnt <= n) a2949_step();
    return (seq_t)A2949_T[n];
}
