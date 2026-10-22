/*
 * A007133  Number of unlabeled rooted connected strength 3 Eulerian graphs
 *          with n nodes.
 * https://oeis.org/A007133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unlabeled rooted connected Eulerian multigraphs
 *            with edge multiplicities 0..3, computed by Burnside/Polya
 *            counting. For a vertex permutation with cycles c_1..c_k, the
 *            number of fixed even-degree weightings is 2^(2E - k + [some
 *            cycle is odd]), where E = Sum floor(c_i/2) + Sum_{i<j...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   gcd_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_i.h"

typedef struct { uint32_t *d; size_t n, cap; } bn;

static void bn_reserve(bn *a, size_t c) {
    if (c > a->cap) {
        size_t nc = c * 2;
        a->d = (uint32_t *)realloc(a->d, nc * sizeof(uint32_t));
        memset(a->d + a->cap, 0, (nc - a->cap) * sizeof(uint32_t));
        a->cap = nc;
    }
}
static void bn_trim(bn *a) { while (a->n && a->d[a->n - 1] == 0) a->n--; }
static void bn_setu(bn *a, uint32_t v) {
    bn_reserve(a, 1);
    if (a->n) memset(a->d, 0, a->n * sizeof(uint32_t));
    a->d[0] = v;
    a->n = v ? 1 : 0;
}
static void bn_copy(bn *d, const bn *s) {
    bn_reserve(d, s->n + 1);
    if (d->n > s->n) memset(d->d + s->n, 0, (d->n - s->n) * sizeof(uint32_t));
    if (s->n) memcpy(d->d, s->d, s->n * sizeof(uint32_t));
    d->n = s->n;
}
static void bn_add_shift(bn *a, const bn *x, unsigned e) {
    size_t w = e / 32, i;
    unsigned b = e % 32;
    size_t need = (a->n > x->n + w + 1 ? a->n : x->n + w + 1) + 1;
    uint64_t carry = 0;
    bn_reserve(a, need);
    for (i = 0; i < x->n; i++) {
        uint64_t xv = (uint64_t)x->d[i] << b;
        uint64_t s = (uint64_t)a->d[w + i] + (uint32_t)xv + carry;
        a->d[w + i] = (uint32_t)s;
        carry = (s >> 32) + (xv >> 32);
    }
    for (i = w + x->n; carry; i++) {
        uint64_t s = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)s;
        carry = s >> 32;
    }
    a->n = need;
    bn_trim(a);
}
static void bn_mul_small(bn *a, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    bn_reserve(a, a->n + 2);
    for (i = 0; i < a->n; i++) {
        uint64_t s = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)s;
        carry = s >> 32;
    }
    if (carry) a->d[a->n++] = (uint32_t)carry;
    bn_trim(a);
}
static void bn_div_small(bn *a, uint32_t m) {
    uint64_t rem = 0;
    size_t i = a->n;
    while (i-- > 0) {
        uint64_t cur = (rem << 32) | a->d[i];
        a->d[i] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    bn_trim(a);
}
static void bn_sub(bn *a, const bn *x) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < a->n; i++) {
        int64_t s = (int64_t)a->d[i] - (i < x->n ? (int64_t)x->d[i] : 0) - borrow;
        if (s < 0) { s += ((int64_t)1 << 32); borrow = 1; } else borrow = 0;
        a->d[i] = (uint32_t)s;
    }
    bn_trim(a);
}
static void bn_mul(bn *r, const bn *a, const bn *b) {
    size_t i, j, tot = a->n + b->n + 1;
    bn_reserve(r, tot + 1);
    memset(r->d, 0, (r->n > tot ? r->n : tot) * sizeof(uint32_t));
    for (i = 0; i < a->n; i++) {
        uint64_t carry = 0;
        for (j = 0; j < b->n; j++) {
            uint64_t s = (uint64_t)a->d[i] * b->d[j] + r->d[i + j] + carry;
            r->d[i + j] = (uint32_t)s;
            carry = s >> 32;
        }
        j = i + b->n;
        while (carry) {
            uint64_t s = (uint64_t)r->d[j] + carry;
            r->d[j] = (uint32_t)s;
            carry = s >> 32;
            j++;
        }
    }
    r->n = tot;
    bn_trim(r);
}

static bn g_S, g_fact, g_tc;
static int g_p[512], g_mu[512], g_k;
static int g_rooted;

static void leaf(void) {
    int pp[513], mm[513], t = 0, j, l, r, found = 0, hasodd = 0;
    long long E = 0, c = 0, ex;
    for (j = 0; j < g_k; j++) { pp[t] = g_p[j]; mm[t] = g_mu[j]; t++; }
    if (g_rooted) {
        for (j = 0; j < t; j++) if (pp[j] == 1) { mm[j]++; found = 1; }
        if (!found) { pp[t] = 1; mm[t] = 1; t++; }
    }
    for (j = 0; j < t; j++) {
        long long p = pp[j], m = mm[j];
        c += m;
        if (p & 1) hasodd = 1;
        E += m * (p / 2) + p * m * (m - 1) / 2;
        for (l = j + 1; l < t; l++) E += m * (long long)mm[l] * gcd_i(pp[j], pp[l]);
    }
    ex = 2 * E - c + hasodd;
    bn_copy(&g_tc, &g_fact);
    for (j = 0; j < g_k; j++) {
        for (r = 0; r < g_mu[j]; r++) bn_div_small(&g_tc, (uint32_t)g_p[j]);
        for (r = 2; r <= g_mu[j]; r++) bn_div_small(&g_tc, (uint32_t)r);
    }
    bn_add_shift(&g_S, &g_tc, (unsigned)ex);
}

static void gen(int rem, int maxp) {
    int p, cnt;
    if (rem == 0) { leaf(); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        for (cnt = 1; p * cnt <= rem; cnt++) {
            g_p[g_k] = p; g_mu[g_k] = cnt; g_k++;
            gen(rem - p * cnt, p - 1);
            g_k--;
        }
    }
}

static void total(int m, int rooted, bn *out) {
    int i;
    g_rooted = rooted;
    g_k = 0;
    bn_setu(&g_fact, 1);
    for (i = 2; i <= m; i++) bn_mul_small(&g_fact, (uint32_t)i);
    bn_setu(&g_S, 0);
    gen(m, m);
    for (i = 2; i <= m; i++) bn_div_small(&g_S, (uint32_t)i);
    bn_copy(out, &g_S);
}

static bn *U_, *R_, *C_;
static size_t done_ = 0, capn_ = 0;

seq_t a(idx_t n) {
    size_t k, j;
    seq_t x = 0;
    if (n >= capn_) {
        size_t nc = (size_t)n + 16;
        U_ = (bn *)realloc(U_, nc * sizeof(bn));
        R_ = (bn *)realloc(R_, nc * sizeof(bn));
        C_ = (bn *)realloc(C_, nc * sizeof(bn));
        memset(U_ + capn_, 0, (nc - capn_) * sizeof(bn));
        memset(R_ + capn_, 0, (nc - capn_) * sizeof(bn));
        memset(C_ + capn_, 0, (nc - capn_) * sizeof(bn));
        if (capn_ == 0) bn_setu(&U_[0], 1);
        capn_ = nc;
    }
    for (k = done_ + 1; k <= (size_t)n; k++) {
        bn tmp;
        tmp.d = NULL; tmp.n = 0; tmp.cap = 0;
        total((int)k, 0, &U_[k]);
        total((int)k - 1, 1, &R_[k]);
        bn_copy(&C_[k], &R_[k]);
        for (j = 1; j < k; j++) {
            bn_mul(&tmp, &C_[j], &U_[k - j]);
            bn_sub(&C_[k], &tmp);
        }
        free(tmp.d);
        done_ = k;
    }
    {
        const bn *v = &C_[n];
        size_t i = v->n;
        while (i-- > 0) {
            int by;
            for (by = 3; by >= 0; by--) {
                x = (seq_t)(x << 8);
                x += (seq_t)((v->d[i] >> (8 * by)) & 255u);
            }
        }
    }
    return x;
}
