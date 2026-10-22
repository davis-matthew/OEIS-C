/*
 * A001424  Number of nonisomorphic and nonantiisomorphic groupoids with n
 *          elements.
 * https://oeis.org/A001424
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A001329(n) + A029850(n))/2, computed by Burnside's lemma
 *            over S_n x C_2: a(n) = (1/(2*n!)) * Sum_{sigma in S_n}
 *            (fix(sigma) + fix(sigma o transposition)), summed over the cycle
 *            types of sigma. For an orbit of length L on the n^2 cell
 *            positions, the cell value can be chosen in f(L) = Sum_{...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   a1424_gcd, a1424_is_big, a1424_mul, a1424_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1424_gcd.h"
#include "../utils/a1424_is_big.h"
#include "../utils/a1424_mul.h"
#include "../utils/a1424_pw.h"

typedef struct {
    idx_t n;
    int big;
    idx_t *sz;
    idx_t *mu;
    int k;
    uint64_t *fb;
    uint64_t *fe;
    int nf;
    seq_t S;
    seq_t fact;
    seq_t Q;
    uint64_t R;
    uint64_t D;
    uint64_t fact64;
} a1424_ctx;

static uint64_t a1424_f(const a1424_ctx *c, uint64_t L)
{
    uint64_t s = 0;
    int j;
    for (j = 0; j < c->k; j++)
        if (L % c->sz[j] == 0) s += c->sz[j] * c->mu[j];
    return s;
}

static void a1424_push(a1424_ctx *c, uint64_t base, uint64_t e)
{
    if (e == 0) return;
    c->fb[c->nf] = base;
    c->fe[c->nf] = e;
    c->nf++;
}

static void a1424_fx(const a1424_ctx *c, int from, int to, seq_t *Q, uint64_t *R)
{
    seq_t q = 0;
    uint64_t r = 1, e;
    int i;
    for (i = from; i < to; i++) {
        uint64_t b = c->fb[i];
        if (b == 0) { *Q = 0; *R = 0; return; }
        if (b == 1) continue;
        for (e = 0; e < c->fe[i]; e++) a1424_mul(&q, &r, b, c->D);
    }
    *Q = q;
    *R = r;
}

static seq_t a1424_bg(const a1424_ctx *c, int from, int to)
{
    seq_t v = 1;
    int i;
    for (i = from; i < to; i++) {
        if (c->fb[i] == 0) return 0;
        if (c->fb[i] == 1) continue;
        v *= a1424_pw(c->fb[i], c->fe[i]);
    }
    return v;
}

static void a1424_leaf(a1424_ctx *c)
{
    int i, j, k = c->k, n1;
    c->nf = 0;
    /* fix(sigma): ordered pairs of cycles */
    for (i = 0; i < k; i++)
        for (j = 0; j < k; j++) {
            uint64_t a = c->sz[i], b = c->sz[j], g = a1424_gcd(a, b);
            a1424_push(c, a1424_f(c, a / g * b), c->mu[i] * c->mu[j] * g);
        }
    n1 = c->nf;
    /* fix(sigma o transposition): pi(x,y) = (sigma y, sigma x) */
    for (i = 0; i < k; i++) {
        uint64_t a = c->sz[i], ma = c->mu[i];
        uint64_t ap = (a & 1) ? a : a / 2;
        if (a & 1) {
            a1424_push(c, a1424_f(c, a), ma);
            a1424_push(c, a1424_f(c, 2 * a), ma * ((a - 1) / 2));
        } else {
            a1424_push(c, a1424_f(c, a), ma * (a - 1));
            if (a % 4 == 2)
                a1424_push(c, a1424_f(c, a / 2), 2 * ma);
            else
                a1424_push(c, a1424_f(c, a), ma);
        }
        a1424_push(c, a1424_f(c, 2 * ap), (ma * (ma - 1) / 2) * (a * a / ap));
        for (j = i + 1; j < k; j++) {
            uint64_t b = c->sz[j], mb = c->mu[j];
            uint64_t bp = (b & 1) ? b : b / 2;
            uint64_t L2 = ap / a1424_gcd(ap, bp) * bp;
            a1424_push(c, a1424_f(c, 2 * L2), ma * mb * (a * b / L2));
        }
    }
    if (c->big) {
        seq_t z = 1;
        for (i = 0; i < k; i++) {
            idx_t t;
            z *= a1424_pw(c->sz[i], c->mu[i]);
            for (t = 2; t <= c->mu[i]; t++) z *= (seq_t)t;
        }
        c->S += (a1424_bg(c, 0, n1) + a1424_bg(c, n1, c->nf)) * (c->fact / z);
    } else {
        seq_t q1, q2, q;
        uint64_t r1, r2, r, z = 1, w;
        for (i = 0; i < k; i++) {
            idx_t t;
            for (t = 0; t < c->mu[i]; t++) z *= c->sz[i];
            for (t = 2; t <= c->mu[i]; t++) z *= t;
        }
        w = c->fact64 / z;
        a1424_fx(c, 0, n1, &q1, &r1);
        a1424_fx(c, n1, c->nf, &q2, &r2);
        q = q1 + q2;
        r = r1 + r2;
        if (r >= c->D) { r -= c->D; q += 1; }
        a1424_mul(&q, &r, w, c->D);
        c->Q += q;
        c->R += r;
        if (c->R >= c->D) { c->R -= c->D; c->Q += 1; }
    }
}

static void a1424_rec(a1424_ctx *c, idx_t rem, idx_t maxp)
{
    idx_t p, m;
    if (rem == 0) { a1424_leaf(c); return; }
    for (p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        for (m = 1; m * p <= rem; m++) {
            c->sz[c->k] = p;
            c->mu[c->k] = m;
            c->k++;
            a1424_rec(c, rem - m * p, p - 1);
            c->k--;
        }
    }
}

seq_t a(idx_t n)
{
    a1424_ctx c;
    idx_t kmax = 1, t;
    size_t cap;
    seq_t res;
    c.n = n;
    c.big = a1424_is_big();
    if (!c.big && n > 10) return 0;
    while (kmax * (kmax + 1) / 2 <= n) kmax++;
    cap = (size_t)(2 * kmax * kmax + 4 * kmax + 4);
    c.sz = (idx_t *)malloc(sizeof(idx_t) * (size_t)(kmax + 1));
    c.mu = (idx_t *)malloc(sizeof(idx_t) * (size_t)(kmax + 1));
    c.fb = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    c.fe = (uint64_t *)malloc(sizeof(uint64_t) * cap);
    c.k = 0;
    c.nf = 0;
    c.S = 0;
    c.fact = 1;
    c.Q = 0;
    c.R = 0;
    c.fact64 = 1;
    if (c.big) {
        for (t = 2; t <= n; t++) c.fact *= (seq_t)t;
    } else {
        for (t = 2; t <= n; t++) c.fact64 *= t;
    }
    c.D = 2 * c.fact64;
    a1424_rec(&c, n, n);
    if (c.big)
        res = c.S / (c.fact * 2);
    else
        res = c.Q;
    free(c.sz);
    free(c.mu);
    free(c.fb);
    free(c.fe);
    return res;
}
