/*
 * A005484  Numerators of continued fraction convergents to cube root of 7.
 * https://oeis.org/A005484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = q(n)*a(n-1) + a(n-2), a(-1) = 1, a(-2) = 0, where q(n)
 *            are the partial quotients of 7^(1/3), found exactly by
 *            Lagrange's method on x^3 - 7
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 36, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static uint32_t *A5484_C[4];
static uint32_t *A5484_T[2];
static size_t A5484_L = 0;
static uint32_t *A5484_pq = NULL;
static size_t A5484_npq = 0, A5484_cap = 0;

static int A5484_neg(const uint32_t *x) { return (int)(x[A5484_L - 1] >> 31); }

static void A5484_resize(size_t nl) {
    int i;
    for (i = 0; i < 4; i++) {
        uint32_t s = A5484_neg(A5484_C[i]) ? 0xFFFFFFFFu : 0u;
        size_t j;
        A5484_C[i] = (uint32_t *)realloc(A5484_C[i], nl * sizeof(uint32_t));
        for (j = A5484_L; j < nl; j++) A5484_C[i][j] = s;
    }
    for (i = 0; i < 2; i++)
        A5484_T[i] = (uint32_t *)realloc(A5484_T[i], nl * sizeof(uint32_t));
    A5484_L = nl;
}

static void A5484_init(void) {
    int i;
    size_t j;
    A5484_L = 8;
    for (i = 0; i < 4; i++) A5484_C[i] = (uint32_t *)calloc(A5484_L, sizeof(uint32_t));
    for (i = 0; i < 2; i++) A5484_T[i] = (uint32_t *)calloc(A5484_L, sizeof(uint32_t));
    A5484_C[3][0] = 1;
    for (j = 0; j < A5484_L; j++) A5484_C[0][j] = 0xFFFFFFFFu;
    A5484_C[0][0] = (uint32_t)(0u - 7u);
}

static void A5484_ensure_room(void) {
    for (;;) {
        bool ok = true;
        int i;
        for (i = 0; i < 4 && ok; i++) {
            uint32_t s = A5484_neg(A5484_C[i]) ? 0xFFFFFFFFu : 0u;
            size_t j;
            for (j = A5484_L - 5; j < A5484_L; j++)
                if (A5484_C[i][j] != s) { ok = false; break; }
            if (ok && ((A5484_C[i][A5484_L - 6] >> 31) != (s & 1u))) ok = false;
        }
        if (ok) return;
        A5484_resize(A5484_L + 8);
    }
}

/* dst += src * q  (mod 2^(32L), two's complement) */
static void A5484_muladd(uint32_t *dst, const uint32_t *src, uint32_t q) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < A5484_L; i++) {
        uint64_t t = (uint64_t)src[i] * q + dst[i] + carry;
        dst[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

/* true if q lies below the unique real root of the current cubic */
static bool A5484_below(uint32_t q) {
    uint32_t *cur = A5484_C[3];
    int k, b = 0;
    for (k = 2; k >= 0; k--) {
        uint32_t *out = A5484_T[b];
        memcpy(out, A5484_C[k], A5484_L * sizeof(uint32_t));
        A5484_muladd(out, cur, q);
        cur = out;
        b ^= 1;
    }
    return A5484_neg(cur) != A5484_neg(A5484_C[3]);
}

static void A5484_step(void) {
    uint64_t lo = 1, hi = 2;
    uint32_t q;
    int i, j;
    uint32_t *t;
    A5484_ensure_room();
    while (hi < 0xFFFFFFFFull && A5484_below((uint32_t)hi)) { lo = hi; hi *= 2; }
    if (hi > 0xFFFFFFFFull) hi = 0xFFFFFFFFull;
    while (hi - lo > 1) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A5484_below((uint32_t)mid)) lo = mid; else hi = mid;
    }
    q = (uint32_t)lo;
    /* Taylor shift: P(x) -> P(x + q) */
    for (i = 0; i <= 2; i++)
        for (j = 2; j >= i; j--)
            A5484_muladd(A5484_C[j], A5484_C[j + 1], q);
    /* reverse: P(y) -> y^3 P(1/y) */
    t = A5484_C[0]; A5484_C[0] = A5484_C[3]; A5484_C[3] = t;
    t = A5484_C[1]; A5484_C[1] = A5484_C[2]; A5484_C[2] = t;
    if (A5484_npq == A5484_cap) {
        A5484_cap = A5484_cap ? 2 * A5484_cap : 64;
        A5484_pq = (uint32_t *)realloc(A5484_pq, A5484_cap * sizeof(uint32_t));
    }
    A5484_pq[A5484_npq++] = q;
}

seq_t a(idx_t n) {
    seq_t p2 = 0, p1 = 1;
    idx_t k;
    if (A5484_L == 0) A5484_init();
    while ((idx_t)A5484_npq <= n) A5484_step();
    for (k = 0; k <= n; k++) {
        seq_t p = (seq_t)A5484_pq[k] * p1 + p2;
        p2 = p1;
        p1 = p;
    }
    return p1;
}
