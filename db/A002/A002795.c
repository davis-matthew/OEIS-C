/*
 * A002795  Denominators of convergents to Lehmer's constant.
 * https://oeis.org/A002795
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   Lehmer's constant xi = cot(Sum_{k>=0} (-1)^k arccot(c_k)),
 *            c(0)=0, c(k)=c(k-1)^2+c(k-1)+1. Partial values r(K) (r(1)=1;
 *            r(K)=(P*c-Q)/(P+Q*c) for K even, (P*c+Q)/(Q*c-P) for K odd)
 *            bracket xi; the common continued-fraction prefix of r(K), r(K+1)
 *            gives xi = [b0;b1,b2,...]. q(j) = b(j)*q(j-1) + q(j-...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   a2795_big, a2795_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a2795_big.h"
#include "../utils/a2795_is_big.h"

#define L2795_N 128

/* small fixed-capacity bignums for fixed-width builds */
static int l2795_len(const uint32_t *x)
{
    int i = L2795_N;
    while (i > 0 && x[i - 1] == 0) i--;
    return i;
}

static int l2795_cmp(const uint32_t *a, const uint32_t *b)
{
    int i;
    for (i = L2795_N - 1; i >= 0; i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void l2795_set(uint32_t *x, uint32_t v)
{
    memset(x, 0, sizeof(uint32_t) * L2795_N);
    x[0] = v;
}

static void l2795_add(uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < L2795_N; i++) {
        uint64_t t = (uint64_t)a[i] + b[i] + carry;
        r[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void l2795_sub(uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    uint64_t borrow = 0;
    int i;
    for (i = 0; i < L2795_N; i++) {
        uint64_t t = (uint64_t)a[i] - b[i] - borrow;
        r[i] = (uint32_t)t;
        borrow = (t >> 32) ? 1 : 0;
    }
}

static void l2795_mul(uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    uint32_t tmp[L2795_N];
    int la = l2795_len(a), lb = l2795_len(b), i, j;
    memset(tmp, 0, sizeof(tmp));
    for (i = 0; i < la; i++) {
        uint64_t carry = 0;
        for (j = 0; j < lb && i + j < L2795_N; j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + tmp[i + j] + carry;
            tmp[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        if (i + lb < L2795_N) tmp[i + lb] = (uint32_t)carry;
    }
    memcpy(r, tmp, sizeof(tmp));
}

static void l2795_divmod(uint32_t *q, uint32_t *r, const uint32_t *a, const uint32_t *b)
{
    uint32_t qq[L2795_N], rr[L2795_N];
    int L = l2795_len(a), bit, i;
    memset(qq, 0, sizeof(qq));
    memset(rr, 0, sizeof(rr));
    for (bit = L * 32 - 1; bit >= 0; bit--) {
        for (i = L2795_N - 1; i > 0; i--) rr[i] = (rr[i] << 1) | (rr[i - 1] >> 31);
        rr[0] = (rr[0] << 1) | ((a[bit / 32] >> (bit % 32)) & 1u);
        if (l2795_cmp(rr, b) >= 0) {
            l2795_sub(rr, rr, b);
            qq[bit / 32] |= (uint32_t)1u << (bit % 32);
        }
    }
    memcpy(q, qq, sizeof(qq));
    memcpy(r, rr, sizeof(rr));
}

static seq_t l2795_to_seq(const uint32_t *x)
{
    seq_t v = 0;
    int i, by;
    for (i = l2795_len(x) - 1; i >= 0; i--) {
        for (by = 3; by >= 0; by--) {
            v = (seq_t)(v * 256);
            v = (seq_t)(v + ((x[i] >> (8 * by)) & 255u));
        }
    }
    return v;
}

static seq_t a2795_fixed(idx_t n)
{
    uint32_t c[L2795_N], P[L2795_N], Q[L2795_N], c2[L2795_N], P2[L2795_N], Q2[L2795_N];
    uint32_t t[L2795_N], u[L2795_N], x0[L2795_N], x1[L2795_N], y0[L2795_N], y1[L2795_N];
    uint32_t bx[L2795_N], rx[L2795_N], by[L2795_N], ry[L2795_N];
    idx_t need = n + 2;
    int k;
    if (n > 7) return 0; /* a(8) exceeds 128 bits */
    l2795_set(c, 1); l2795_set(P, 1); l2795_set(Q, 1);
    for (k = 1; k < 11; k++) {
        l2795_mul(t, c, c); l2795_add(t, t, c); l2795_set(u, 1); l2795_add(c2, t, u);
        if ((k + 1) % 2 == 0) {
            l2795_mul(t, P, c2); l2795_sub(P2, t, Q);
            l2795_mul(u, Q, c2); l2795_add(Q2, P, u);
        } else {
            l2795_mul(t, P, c2); l2795_add(P2, t, Q);
            l2795_mul(u, Q, c2); l2795_sub(Q2, u, P);
        }
        memcpy(x0, P, sizeof(x0)); memcpy(x1, Q, sizeof(x1));
        memcpy(y0, P2, sizeof(y0)); memcpy(y1, Q2, sizeof(y1));
        {
            seq_t qa = 1, qb = 0;
            idx_t got = 0;
            int ok = 1;
            while (got < need) {
                seq_t b, tt;
                if (l2795_len(x1) == 0 || l2795_len(y1) == 0) { ok = 0; break; }
                l2795_divmod(bx, rx, x0, x1);
                l2795_divmod(by, ry, y0, y1);
                if (l2795_cmp(bx, by) != 0 || l2795_len(rx) == 0 || l2795_len(ry) == 0) { ok = 0; break; }
                b = l2795_to_seq(bx);
                tt = (seq_t)(b * qb + qa); qa = qb; qb = tt;
                memcpy(x0, x1, sizeof(x0)); memcpy(x1, rx, sizeof(x1));
                memcpy(y0, y1, sizeof(y0)); memcpy(y1, ry, sizeof(y1));
                got++;
            }
            if (ok) return qb;
        }
        memcpy(c, c2, sizeof(c)); memcpy(P, P2, sizeof(P)); memcpy(Q, Q2, sizeof(Q));
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (a2795_is_big()) return a2795_big(n);
    return a2795_fixed(n);
}
