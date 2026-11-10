/*
 * A085768  Smallest m such that n divides A000055(m).
 * https://oeis.org/A085768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m >= 0 such that n divides A000055(m), where
 *            A000055(m) = r(m) - Sum_{1<=i<=j, i+j=m} r(i)*r(j) + [m
 *            even]*binomial(r(m/2)+1,2), r = A000081: r(1)=1, r(k+1) =
 *            (1/k)*Sum_{j=1..k} s(j)*r(k+1-j), s(j) = Sum_{d|j} d*r(d).
 *            Computed by search over m with exact fixed-size bignums, reduced
 *            m...
 * Range:     u8 n <= 65, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define A85768_BL 80
#define A85768_MAXM 1400

typedef struct { uint32_t v[A85768_BL]; } a85768_bn;

static int a85768_len(const a85768_bn *a)
{
    int i = A85768_BL;
    while (i > 0 && a->v[i - 1] == 0) i--;
    return i;
}

static void a85768_zero(a85768_bn *a) { memset(a->v, 0, sizeof a->v); }

static void a85768_add(a85768_bn *a, const a85768_bn *b)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < A85768_BL; i++) {
        c += (uint64_t)a->v[i] + b->v[i];
        a->v[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void a85768_sub(a85768_bn *a, const a85768_bn *b)
{
    uint64_t br = 0;
    int i;
    for (i = 0; i < A85768_BL; i++) {
        uint64_t t = (uint64_t)a->v[i] - b->v[i] - br;
        a->v[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

static void a85768_mul(a85768_bn *c, const a85768_bn *a, const a85768_bn *b)
{
    a85768_bn r;
    int i, j;
    int la = a85768_len(a), lb = a85768_len(b);
    a85768_zero(&r);
    for (i = 0; i < la; i++) {
        uint64_t carry = 0;
        if (a->v[i] == 0) continue;
        for (j = 0; j < lb && i + j < A85768_BL; j++) {
            uint64_t t = (uint64_t)a->v[i] * b->v[j] + r.v[i + j] + carry;
            r.v[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
        if (i + lb < A85768_BL) r.v[i + lb] += (uint32_t)carry;
    }
    *c = r;
}

static void a85768_muls(a85768_bn *a, uint32_t m)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < A85768_BL; i++) {
        uint64_t t = (uint64_t)a->v[i] * m + carry;
        a->v[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void a85768_divs(a85768_bn *a, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = A85768_BL - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a->v[i];
        a->v[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static uint64_t a85768_mod(const a85768_bn *a, uint64_t n)
{
    unsigned __int128 rem = 0;
    int i;
    for (i = A85768_BL - 1; i >= 0; i--) {
        rem = ((rem << 32) | a->v[i]) % n;
    }
    return (uint64_t)rem;
}

static a85768_bn *A85768_R = 0, *A85768_S = 0, *A85768_T = 0;
static int A85768_have = 0;   /* r,s valid for indices <= have */
static int A85768_tnum = 0;   /* T valid for indices < tnum */

static void a85768_init(void)
{
    if (A85768_R) return;
    A85768_R = (a85768_bn *)malloc((A85768_MAXM + 2) * sizeof(a85768_bn));
    A85768_S = (a85768_bn *)malloc((A85768_MAXM + 2) * sizeof(a85768_bn));
    A85768_T = (a85768_bn *)malloc((A85768_MAXM + 2) * sizeof(a85768_bn));
    a85768_zero(&A85768_R[0]);
    a85768_zero(&A85768_S[0]);
    a85768_zero(&A85768_R[1]);
    a85768_zero(&A85768_S[1]);
    A85768_R[1].v[0] = 1;
    A85768_S[1].v[0] = 1;
    A85768_have = 1;
}

static void a85768_ensure_rs(int N)
{
    int m, k, d;
    a85768_bn acc, t;
    while (A85768_have < N) {
        m = A85768_have;
        a85768_zero(&acc);
        for (k = 1; k <= m; k++) {
            a85768_mul(&t, &A85768_S[k], &A85768_R[m + 1 - k]);
            a85768_add(&acc, &t);
        }
        a85768_divs(&acc, (uint32_t)m);
        A85768_R[m + 1] = acc;
        a85768_zero(&A85768_S[m + 1]);
        for (d = 1; d * d <= m + 1; d++)
            if ((m + 1) % d == 0) {
                int e = (m + 1) / d;
                t = A85768_R[d];
                a85768_muls(&t, (uint32_t)d);
                a85768_add(&A85768_S[m + 1], &t);
                if (e != d) {
                    t = A85768_R[e];
                    a85768_muls(&t, (uint32_t)e);
                    a85768_add(&A85768_S[m + 1], &t);
                }
            }
        A85768_have = m + 1;
    }
}

static void a85768_compute_t(int N)
{
    a85768_bn acc, t, h, one;
    int i;
    a85768_ensure_rs(N < 1 ? 1 : N);
    if (N == 0) {
        a85768_zero(&A85768_T[0]);
        A85768_T[0].v[0] = 1;
        return;
    }
    acc = A85768_R[N];
    for (i = 1; 2 * i <= N; i++) {
        a85768_mul(&t, &A85768_R[i], &A85768_R[N - i]);
        a85768_sub(&acc, &t);
    }
    if (N % 2 == 0) {
        h = A85768_R[N / 2];
        a85768_zero(&one);
        one.v[0] = 1;
        t = h;
        a85768_add(&t, &one);
        a85768_mul(&t, &t, &h);
        a85768_divs(&t, 2);
        a85768_add(&acc, &t);
    }
    A85768_T[N] = acc;
}

seq_t a(idx_t n)
{
    int m;
    uint64_t nn = (uint64_t)n;
    a85768_init();
    if (nn <= 1) return (seq_t)0;
    for (m = 0; m <= A85768_MAXM; m++) {
        while (A85768_tnum <= m) {
            a85768_compute_t(A85768_tnum);
            A85768_tnum++;
        }
        if (a85768_mod(&A85768_T[m], nn) == 0) return (seq_t)m;
    }
    return (seq_t)0;
}
