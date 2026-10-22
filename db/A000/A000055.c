/*
 * A000055  Number of trees with n unlabeled nodes.
 * https://oeis.org/A000055
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, core, changed
 * Formula:   a(n) = A000081(n) - Sum_{1<=i<=j, i+j=n} A000081(i)*A000081(j) +
 *            [n even] * binomial(A000081(n/2)+1, 2), a(0) = 1, where r =
 *            A000081: r(1) = 1, r(m+1) = (1/m) * Sum_{k=1..m} s(k)*r(m+1-k),
 *            s(k) = Sum_{d|k} d*r(d)
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 50, u128 n <= 92,
 *            big any n
 * Helpers:   alloc_series, width_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_series.h"
#include "../utils/width_bits.h"

/* ---------- small fixed-size bignum, used for fixed-width seq_t ---------- */
#define BL 16
typedef struct { uint32_t v[BL]; } bnum_t;

static void bn_zero(bnum_t *a) { memset(a->v, 0, sizeof a->v); }
static void bn_set(bnum_t *a, uint32_t x) { bn_zero(a); a->v[0] = x; }

static void bn_add(bnum_t *a, const bnum_t *b)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < BL; i++) {
        c += (uint64_t)a->v[i] + b->v[i];
        a->v[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void bn_sub(bnum_t *a, const bnum_t *b)
{
    uint64_t br = 0;
    int i;
    for (i = 0; i < BL; i++) {
        uint64_t t = (uint64_t)a->v[i] - b->v[i] - br;
        a->v[i] = (uint32_t)t;
        br = (t >> 63) & 1;
    }
}

static void bn_mul(bnum_t *c, const bnum_t *a, const bnum_t *b)
{
    bnum_t r;
    int i, j;
    bn_zero(&r);
    for (i = 0; i < BL; i++) {
        uint64_t carry = 0;
        if (a->v[i] == 0) continue;
        for (j = 0; i + j < BL; j++) {
            uint64_t t = (uint64_t)a->v[i] * b->v[j] + r.v[i + j] + carry;
            r.v[i + j] = (uint32_t)t;
            carry = t >> 32;
        }
    }
    *c = r;
}

static void bn_muls(bnum_t *a, uint32_t m)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < BL; i++) {
        uint64_t t = (uint64_t)a->v[i] * m + carry;
        a->v[i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void bn_divs(bnum_t *a, uint32_t d)
{
    uint64_t rem = 0;
    int i;
    for (i = BL - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | a->v[i];
        a->v[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static seq_t bn_to_seq(const bnum_t *a)
{
    seq_t res = 0;
    int i, b;
    for (i = BL - 1; i >= 0; i--)
        for (b = 3; b >= 0; b--)
            res = (seq_t)(res * 256u + (unsigned)((a->v[i] >> (8 * b)) & 255u));
    return res;
}

static seq_t a_fixed(idx_t n)
{
    size_t N = (size_t)n, m, k, d, i;
    bnum_t *r = (bnum_t *)malloc((N + 2) * sizeof(bnum_t));
    bnum_t *s = (bnum_t *)malloc((N + 2) * sizeof(bnum_t));
    bnum_t acc, t, h;
    seq_t out;

    bn_zero(&r[0]);
    bn_zero(&s[0]);
    bn_set(&r[1], 1);
    bn_set(&s[1], 1);
    for (m = 1; m < N; m++) {
        bn_zero(&acc);
        for (k = 1; k <= m; k++) {
            bn_mul(&t, &s[k], &r[m + 1 - k]);
            bn_add(&acc, &t);
        }
        bn_divs(&acc, (uint32_t)m);
        r[m + 1] = acc;
        bn_zero(&s[m + 1]);
        for (d = 1; d <= m + 1; d++)
            if ((m + 1) % d == 0) {
                t = r[d];
                bn_muls(&t, (uint32_t)d);
                bn_add(&s[m + 1], &t);
            }
    }
    acc = r[N];
    for (i = 1; 2 * i <= N; i++) {
        bn_mul(&t, &r[i], &r[N - i]);
        bn_sub(&acc, &t);
    }
    if (N % 2 == 0) {
        bnum_t one;
        h = r[N / 2];
        bn_set(&one, 1);
        t = h;
        bn_add(&t, &one);
        bn_mul(&t, &t, &h);
        bn_divs(&t, 2);
        bn_add(&acc, &t);
    }
    out = bn_to_seq(&acc);
    free(r);
    free(s);
    return out;
}

/* ---------- arbitrary-precision seq_t: memoized recurrence ---------- */
static seq_t *R = 0, *S = 0;
static size_t cap = 0, have = 0;

static void ensure(size_t n)
{
    size_t i, m, k, d;
    if (n + 1 > cap) {
        size_t nc = cap * 2;
        seq_t *R2, *S2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        R2 = alloc_series(nc);
        S2 = alloc_series(nc);
        for (i = 0; i < nc; i++) { R2[i] = 0; S2[i] = 0; }
        for (i = 0; i <= have && i < cap; i++) { R2[i] = R[i]; S2[i] = S[i]; }
        R = R2;
        S = S2;
        cap = nc;
        if (have == 0) {
            R[1] = 1;
            S[1] = 1;
            have = 1;
        }
    }
    while (have < n) {
        seq_t acc = 0, sm = 0;
        m = have;
        for (k = 1; k <= m; k++)
            acc += S[k] * R[m + 1 - k];
        acc /= (seq_t)m;
        R[m + 1] = acc;
        k = m + 1;
        for (d = 1; d * d <= k; d++)
            if (k % d == 0) {
                size_t e = k / d;
                sm += R[d] * (seq_t)d;
                if (e != d) sm += R[e] * (seq_t)e;
            }
        S[m + 1] = sm;
        have = m + 1;
    }
}

static seq_t a_big(idx_t n)
{
    size_t N = (size_t)n, i;
    seq_t res;
    ensure(N);
    res = R[N];
    if (N % 2 == 0) {
        seq_t h = R[N / 2];
        res += h * (h + 1) / 2;
    }
    for (i = 1; 2 * i <= N; i++)
        res -= R[i] * R[N - i];
    return res;
}

seq_t a(idx_t n)
{
    static int w = -1;
    if (n == 0) return 1;
    if (w < 0) w = width_bits();
    if (w >= 300) return a_big(n);
    /* for n > w + 30 the term certainly exceeds 2^w */
    if (n > (idx_t)(w + 30)) return 0;
    return a_fixed(n);
}
