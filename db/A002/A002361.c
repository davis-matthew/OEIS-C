/*
 * A002361  Denominators of continued fraction convergents to fifth root of 2.
 * https://oeis.org/A002361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(n) are
 *            the partial quotients of 2^(1/5). They are computed exactly from
 *            x^5 - 2 using Lagrange's method: each step finds floor of the
 *            polynomial's unique real root, applies a Taylor shift x -> x +
 *            c, then reverses (x -> 1/x)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 33, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Polynomial coefficients as fixed-length two's complement big integers */
static uint32_t *P[6];
static uint32_t *T1, *T2;
static size_t L = 0;
static uint64_t *pq = NULL;
static size_t npq = 0, pqcap = 0;

static void cf_init(void) {
    int k;
    size_t j;
    L = 16;
    for (k = 0; k < 6; k++) P[k] = (uint32_t *)calloc(L, sizeof(uint32_t));
    T1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    T2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    P[5][0] = 1;
    for (j = 0; j < L; j++) P[0][j] = 0xFFFFFFFFu;
    P[0][0] = 0xFFFFFFFEu; /* -2 */
}

static void ensure_margin(void) {
    for (;;) {
        int ok = 1, k;
        size_t j;
        for (k = 0; k < 6 && ok; k++) {
            uint32_t *x = P[k];
            uint32_t ext = (x[L - 9] >> 31) ? 0xFFFFFFFFu : 0u;
            for (j = L - 8; j < L; j++)
                if (x[j] != ext) { ok = 0; break; }
        }
        if (ok) return;
        {
            size_t newL = L * 2;
            for (k = 0; k < 6; k++) {
                uint32_t *x = (uint32_t *)realloc(P[k], newL * sizeof(uint32_t));
                uint32_t ext = (x[L - 1] >> 31) ? 0xFFFFFFFFu : 0u;
                for (j = L; j < newL; j++) x[j] = ext;
                P[k] = x;
            }
            T1 = (uint32_t *)realloc(T1, newL * sizeof(uint32_t));
            T2 = (uint32_t *)realloc(T2, newL * sizeof(uint32_t));
            L = newL;
        }
    }
}

/* dst += src * m  (mod 2^(32L)) */
static void addmul(uint32_t *dst, const uint32_t *src, uint32_t m) {
    uint64_t carry = 0;
    size_t j;
    for (j = 0; j < L; j++) {
        uint64_t t = (uint64_t)dst[j] + (uint64_t)src[j] * m + carry;
        dst[j] = (uint32_t)t;
        carry = t >> 32;
    }
}

static int is_neg(const uint32_t *x) { return (int)(x[L - 1] >> 31); }

/* true iff the unique real root exceeds c */
static int gt(uint32_t c) {
    int i;
    uint32_t *v = T1, *w = T2, *t;
    memcpy(v, P[5], L * sizeof(uint32_t));
    for (i = 4; i >= 0; i--) {
        memcpy(w, P[i], L * sizeof(uint32_t));
        addmul(w, v, c);
        t = v; v = w; w = t;
    }
    return is_neg(v) ^ is_neg(P[5]);
}

static void taylor_shift(uint32_t c) {
    int i, j;
    for (i = 0; i < 5; i++)
        for (j = 4; j >= i; j--)
            addmul(P[j], P[j + 1], c);
}

static uint64_t next_pq(void) {
    uint64_t total = 0;
    int k;
    for (;;) {
        uint32_t c, m;
        ensure_margin();
        if (!gt(1)) break;
        c = 1;
        while (c < 0x80000000u && gt(c * 2)) c *= 2;
        if (c >= 0x80000000u) {
            m = c;
        } else {
            uint64_t lo = c, hi = (uint64_t)c * 2;
            while (hi - lo > 1) {
                uint64_t mid = (lo + hi) / 2;
                if (gt((uint32_t)mid)) lo = mid; else hi = mid;
            }
            m = (uint32_t)lo;
        }
        taylor_shift(m);
        total += m;
    }
    for (k = 0; k < 3; k++) {
        uint32_t *t = P[k];
        P[k] = P[5 - k];
        P[5 - k] = t;
    }
    return total;
}

seq_t a(idx_t n) {
    idx_t i;
    seq_t q2, q1;
    if (L == 0) cf_init();
    while (npq <= (size_t)n) {
        if (npq == pqcap) {
            pqcap = pqcap ? pqcap * 2 : 64;
            pq = (uint64_t *)realloc(pq, pqcap * sizeof(uint64_t));
        }
        pq[npq++] = next_pq();
    }
    q2 = 0;
    q1 = 1;
    for (i = 1; i <= n; i++) {
        seq_t q = (seq_t)pq[i] * q1 + q2;
        q2 = q1;
        q1 = q;
    }
    return q1;
}
