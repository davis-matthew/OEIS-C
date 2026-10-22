/*
 * A002353  Denominators of convergents to cube root of 3.
 * https://oeis.org/A002353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(n) are
 *            the partial quotients of the continued fraction of 3^(1/3). Each
 *            c(n) is found exactly from the cubic for the current complete
 *            quotient (starting from x^3 - 3) and substituting x = c + 1/y
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 18, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   a2353_sext
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2353_sext.h"

static uint32_t *a2353_C[4];
static uint32_t *a2353_tmp;
static size_t a2353_L = 0;
static uint32_t *a2353_pq = NULL;
static idx_t a2353_npq = 0, a2353_cap = 0;

static void a2353_init(void) {
    int j;
    a2353_L = 8;
    for (j = 0; j < 4; j++) {
        a2353_C[j] = (uint32_t *)calloc(a2353_L, sizeof(uint32_t));
    }
    a2353_tmp = (uint32_t *)calloc(a2353_L, sizeof(uint32_t));
    /* x^3 - 3 */
    a2353_C[3][0] = 1;
    {
        size_t i;
        uint32_t *c0 = a2353_C[0];
        c0[0] = 3;
        for (i = 0; i < a2353_L; i++) c0[i] = ~c0[i];
        for (i = 0; i < a2353_L; i++) { if (++c0[i] != 0) break; }
    }
}

static void a2353_headroom(void) {
    int j, need = 0;
    size_t i;
    for (j = 0; j < 4 && !need; j++) {
        uint32_t s = a2353_sext(a2353_C[j], a2353_L - 5);
        for (i = a2353_L - 4; i < a2353_L; i++)
            if (a2353_C[j][i] != s) { need = 1; break; }
    }
    if (!need) return;
    {
        size_t NL = a2353_L * 2;
        for (j = 0; j < 4; j++) {
            uint32_t s = a2353_sext(a2353_C[j], a2353_L - 1);
            uint32_t *p = (uint32_t *)realloc(a2353_C[j], NL * sizeof(uint32_t));
            for (i = a2353_L; i < NL; i++) p[i] = s;
            a2353_C[j] = p;
        }
        a2353_tmp = (uint32_t *)realloc(a2353_tmp, NL * sizeof(uint32_t));
        a2353_L = NL;
    }
}

/* dst = dst*k + src  (mod 2^(32L)) */
static void a2353_muladd(uint32_t *dst, uint32_t k, const uint32_t *src) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a2353_L; i++) {
        uint64_t t = (uint64_t)dst[i] * k + src[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

/* dst += src*k  (mod 2^(32L)) */
static void a2353_addmul(uint32_t *dst, const uint32_t *src, uint32_t k) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < a2353_L; i++) {
        uint64_t t = (uint64_t)src[i] * k + dst[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static int a2353_neg_at(uint32_t k) {
    memcpy(a2353_tmp, a2353_C[3], a2353_L * sizeof(uint32_t));
    a2353_muladd(a2353_tmp, k, a2353_C[2]);
    a2353_muladd(a2353_tmp, k, a2353_C[1]);
    a2353_muladd(a2353_tmp, k, a2353_C[0]);
    return (int)(a2353_tmp[a2353_L - 1] >> 31);
}

static void a2353_next(void) {
    uint32_t k = 1, lo, hi, q;
    int i, j;
    a2353_headroom();
    while (k < (1u << 30) && a2353_neg_at(2 * k)) k *= 2;
    lo = k; hi = 2 * k;
    while (hi - lo > 1) {
        uint32_t mid = lo + (hi - lo) / 2;
        if (a2353_neg_at(mid)) lo = mid; else hi = mid;
    }
    q = lo;
    /* Taylor shift: P(x) -> P(x + q) */
    for (i = 0; i < 3; i++)
        for (j = 2; j >= i; j--)
            a2353_addmul(a2353_C[j], a2353_C[j + 1], q);
    /* reverse: y^3 * P(q + 1/y) */
    {
        uint32_t *t0 = a2353_C[0], *t1 = a2353_C[1];
        a2353_C[0] = a2353_C[3];
        a2353_C[3] = t0;
        a2353_C[1] = a2353_C[2];
        a2353_C[2] = t1;
    }
    if (a2353_C[3][a2353_L - 1] >> 31) {
        for (j = 0; j < 4; j++) {
            size_t t;
            uint32_t *c = a2353_C[j];
            for (t = 0; t < a2353_L; t++) c[t] = ~c[t];
            for (t = 0; t < a2353_L; t++) { if (++c[t] != 0) break; }
        }
    }
    if (a2353_npq >= a2353_cap) {
        a2353_cap = a2353_cap ? a2353_cap * 2 : 64;
        a2353_pq = (uint32_t *)realloc(a2353_pq, a2353_cap * sizeof(uint32_t));
    }
    a2353_pq[a2353_npq++] = q;
}

seq_t a(idx_t n) {
    seq_t qprev = 0, qcur = 1;
    idx_t i;
    if (a2353_L == 0) a2353_init();
    while (a2353_npq <= n) a2353_next();
    for (i = 1; i <= n; i++) {
        seq_t qn = (seq_t)a2353_pq[i] * qcur + qprev;
        qprev = qcur;
        qcur = qn;
    }
    return qcur;
}
