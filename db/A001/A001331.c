/*
 * A001331  Number of n-element algebras with 1 ternary operation.
 * https://oeis.org/A001331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/n!) * Sum_{sigma in S_n} Prod_{ordered triples
 *            (c1,c2,c3) of cycles of sigma}
 *            s(lcm(|c1|,|c2|,|c3|))^gcd(|c1|,|c2|,|c3|), where s(L) =
 *            Sum_{d|L} d*m_d and m_d is the number of d-cycles of sigma
 *            (summed over cycle types, weighted by class size n!/z_lambda)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   a1331_add, a1331_div, a1331_gcd, a1331_mul, a1331_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1331_add.h"
#include "../utils/a1331_div.h"
#include "../utils/a1331_gcd.h"
#include "../utils/a1331_mul.h"
#include "../utils/a1331_pow.h"

typedef struct {
    idx_t n;
    uint64_t *m;
    uint64_t *ls;
    uint32_t *S;
    size_t slen;
    uint32_t *T;
} a1331_ctx;

static void a1331_leaf(a1331_ctx *c)
{
    idx_t n = c->n;
    uint64_t *m = c->m;
    size_t tl = 1, k = 0, i, j, t, u;
    uint64_t l, q;
    c->T[0] = 1;
    for (l = 2; l <= n; l++) a1331_mul(c->T, &tl, (uint32_t)l);
    for (l = 1; l <= n; l++)
        for (q = 1; q <= m[l]; q++) {
            a1331_div(c->T, &tl, (uint32_t)l);
            a1331_div(c->T, &tl, (uint32_t)q);
        }
    for (l = 1; l <= n; l++)
        if (m[l]) c->ls[k++] = l;
    for (i = 0; i < k; i++)
        for (j = 0; j < k; j++)
            for (t = 0; t < k; t++) {
                uint64_t p1 = c->ls[i], p2 = c->ls[j], p3 = c->ls[t];
                uint64_t g = a1331_gcd(a1331_gcd(p1, p2), p3);
                uint64_t L12 = p1 / a1331_gcd(p1, p2) * p2;
                uint64_t L = L12 / a1331_gcd(L12, p3) * p3;
                uint64_t s = 0, e;
                for (u = 0; u < k; u++)
                    if (L % c->ls[u] == 0) s += c->ls[u] * m[c->ls[u]];
                e = m[p1] * m[p2] * m[p3] * g;
                a1331_pow(c->T, &tl, s, e);
            }
    a1331_add(c->S, &c->slen, c->T, tl);
}

static void a1331_rec(a1331_ctx *c, uint64_t rem, uint64_t maxp)
{
    uint64_t p;
    if (rem == 0) { a1331_leaf(c); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        c->m[p]++;
        a1331_rec(c, rem - p, p);
        c->m[p]--;
    }
}

seq_t a(idx_t n)
{
    a1331_ctx c;
    uint64_t b = 0, x = n, bits, k;
    size_t cap, i;
    seq_t r = 0;
    while (x) { b++; x >>= 1; }
    bits = (uint64_t)n * n * n * b + (uint64_t)n * b + n + 128;
    cap = (size_t)(bits / 32 + 4);
    c.n = n;
    c.m = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    c.ls = (uint64_t *)calloc((size_t)n + 1, sizeof(uint64_t));
    c.S = (uint32_t *)calloc(cap, sizeof(uint32_t));
    c.T = (uint32_t *)calloc(cap, sizeof(uint32_t));
    c.slen = 0;
    a1331_rec(&c, n, n);
    for (k = 2; k <= n; k++) a1331_div(c.S, &c.slen, (uint32_t)k);
    i = c.slen;
    while (i > 0) {
        int sh;
        i--;
        for (sh = 24; sh >= 0; sh -= 8) {
            r = r * 256;
            r += (seq_t)(unsigned)((c.S[i] >> sh) & 255u);
        }
    }
    free(c.m);
    free(c.ls);
    free(c.S);
    free(c.T);
    return r;
}
