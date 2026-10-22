/*
 * A003225  The number of superpositions of cycles of order n of the groups
 *          S_3 and D_n.
 * https://oeis.org/A003225
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Burnside count of triples of undirected Hamiltonian cycles on n
 *            vertices under S_n x S_3: a(n) = (1/(6 n!)) * Sum_{s in S_n}
 *            [h(s)^3 + 3 h(s) h(s^2) + 2 h(s^3)], where h(s) = number of
 *            Hamiltonian cycles fixed by s. Grouping by the cycle types t of
 *            D_n (c_t elements, centralizer order z_t, h_t =...
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   a3225_crot, a3225_isfixed, a3225_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3225_crot.h"
#include "../utils/a3225_isfixed.h"
#include "../utils/a3225_phi.h"

typedef struct { uint32_t *d; size_t len; } a3225_bn;

static void a3225_set(a3225_bn *x, uint64_t v) {
    x->len = 0;
    while (v) { x->d[x->len++] = (uint32_t)v; v >>= 32; }
}

static void a3225_copy(a3225_bn *x, const a3225_bn *y) {
    memcpy(x->d, y->d, y->len * sizeof(uint32_t));
    x->len = y->len;
}

static void a3225_mul(a3225_bn *x, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    if (m == 0) { x->len = 0; return; }
    for (i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] * m + carry;
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
    }
    while (carry) { x->d[x->len++] = (uint32_t)carry; carry >>= 32; }
}

static void a3225_add(a3225_bn *x, const a3225_bn *y) {
    uint64_t carry = 0;
    size_t i;
    while (x->len < y->len) x->d[x->len++] = 0;
    for (i = 0; i < x->len; i++) {
        uint64_t t = (uint64_t)x->d[i] + carry + (i < y->len ? y->d[i] : 0);
        x->d[i] = (uint32_t)t;
        carry = t >> 32;
        if (i >= y->len && !carry) break;
    }
    if (carry) x->d[x->len++] = (uint32_t)carry;
}

static void a3225_div(a3225_bn *x, uint32_t dv) {
    uint64_t r = 0;
    size_t i = x->len;
    while (i > 0) {
        i--;
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / dv);
        r = cur % dv;
    }
    while (x->len && x->d[x->len - 1] == 0) x->len--;
}

/* multiply by centralizer order of type k^m 1^f: k^m * m! * f! */
static void a3225_mulz(a3225_bn *x, uint32_t k, uint32_t m, uint32_t f) {
    uint32_t i;
    if (k > 1) for (i = 0; i < m; i++) a3225_mul(x, k);
    for (i = 2; i <= m; i++) a3225_mul(x, i);
    for (i = 2; i <= f; i++) a3225_mul(x, i);
}

/* g_k(m) into G[] ring, returns index holding result */
static int a3225_roots(a3225_bn *G, uint32_t k, uint32_t m) {
    uint32_t i;
    a3225_set(&G[0], 1);
    for (i = 1; i <= m; i++) {
        a3225_bn *dst = &G[i % 4];
        if (i >= 3) {
            a3225_copy(dst, &G[(i - 3) % 4]);
            a3225_mul(dst, i - 1);
            a3225_mul(dst, i - 2);
            a3225_mul(dst, k);
            a3225_mul(dst, k);
        } else {
            dst->len = 0;
        }
        if (k % 3 != 0) a3225_add(dst, &G[(i - 1) % 4]);
    }
    return (int)(m % 4);
}

seq_t a(idx_t nn) {
    uint32_t n = (uint32_t)nn, k, i;
    uint64_t S = 0;
    size_t cap;
    a3225_bn N, T, G[4];
    uint32_t tc[64], tk[64], tm[64], tf[64];
    int nt = 0, t, gi;
    seq_t r = 0;

    if (n < 3) return 0;
    for (k = 2; k <= n; k++) {
        uint32_t v = k;
        while (v) { S++; v >>= 1; }
    }
    cap = (size_t)((2 * S) / 32 + 64);
    N.d = (uint32_t *)calloc(cap, sizeof(uint32_t)); N.len = 0;
    T.d = (uint32_t *)calloc(cap, sizeof(uint32_t)); T.len = 0;
    for (i = 0; i < 4; i++) { G[i].d = (uint32_t *)calloc(cap, sizeof(uint32_t)); G[i].len = 0; }

    tc[nt] = 1; tk[nt] = 1; tm[nt] = n; tf[nt] = 0; nt++;
    for (k = 2; k <= n; k++)
        if (n % k == 0) { tc[nt] = a3225_crot(n, k); tk[nt] = k; tm[nt] = n / k; tf[nt] = 0; nt++; }
    if (n % 2) { tc[nt] = n; tk[nt] = 2; tm[nt] = (n - 1) / 2; tf[nt] = 1; nt++; }
    else { tc[nt] = n / 2; tk[nt] = 2; tm[nt] = (n - 2) / 2; tf[nt] = 2; nt++; }

    for (t = 0; t < nt; t++) {
        uint32_t c = tc[t], kk = tk[t], m = tm[t], f = tf[t];
        uint32_t cs, ks, ms, fs;
        /* c^3 z^2 */
        a3225_set(&T, c); a3225_mul(&T, c); a3225_mul(&T, c);
        a3225_mulz(&T, kk, m, f); a3225_mulz(&T, kk, m, f);
        a3225_add(&N, &T);
        /* square type */
        if (kk % 2) { cs = c; ks = kk; ms = m; fs = f; }
        else if (kk == 2) { cs = 1; ks = 1; ms = n; fs = 0; }
        else { ks = kk / 2; ms = 2 * m; fs = f; cs = a3225_crot(n, ks); }
        a3225_set(&T, 6); a3225_mul(&T, n); a3225_mul(&T, c); a3225_mul(&T, cs);
        a3225_mulz(&T, ks, ms, fs);
        a3225_add(&N, &T);
        /* cube roots */
        gi = a3225_roots(G, kk, m);
        a3225_copy(&T, &G[gi]);
        a3225_mul(&T, 8); a3225_mul(&T, n); a3225_mul(&T, n); a3225_mul(&T, c);
        a3225_add(&N, &T);
    }
    a3225_div(&N, 48);
    a3225_div(&N, n); a3225_div(&N, n); a3225_div(&N, n);

    if (a3225_isfixed()) {
        size_t top = N.len < 4 ? N.len : 4;
        size_t j;
        for (j = top; j > 0; j--) {
            for (i = 0; i < 8; i++) r <<= 4;
            r += (seq_t)N.d[j - 1];
        }
    } else {
        size_t j;
        for (j = N.len; j > 0; j--) {
            r <<= 32;
            r += (seq_t)N.d[j - 1];
        }
    }
    free(N.d); free(T.d);
    for (i = 0; i < 4; i++) free(G[i].d);
    return r;
}
