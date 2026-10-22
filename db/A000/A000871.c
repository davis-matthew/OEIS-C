/*
 * A000871  Number of switching networks with S(n) acting on the domain and
 *          GL(2,2) acting on the range.
 * https://oeis.org/A000871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count of maps {0,1}^n -> GF(2)^2 under S(n) x GL(2,2):
 *            a(n) = (1/(6*n!)) * Sum_{sigma in S(n)} ( 4^c(sigma) +
 *            3*Prod_{cycles L}(L even ? 4 : 2) + 2*Prod_{cycles L}(3|L ? 4 :
 *            1) ), where the cycles are those induced by sigma on {0,1}^n;
 *            summed over cycle types (partitions of n), weight n!...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a871_addsh, a871_divs, a871_muls
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a871_addsh.h"
#include "../utils/a871_divs.h"
#include "../utils/a871_muls.h"

typedef struct {
    int n;
    uint32_t *acc;
    int L;
    uint32_t *img;
    unsigned char *vis;
    const uint32_t *fact;
    uint32_t *coef;
    int CL;
    int *sig;
} a871_ctx;

static void a871_leaf(a871_ctx *cx, const int *parts, int k) {
    int n = cx->n, i, j, pos = 0;
    uint32_t N = (uint32_t)1 << n, m;
    uint64_t C = 0, Ce = 0, Co = 0, C3 = 0;
    for (i = 0; i < k; i++) {
        int l = parts[i];
        for (j = 0; j < l; j++) cx->sig[pos + j] = pos + (j + 1) % l;
        pos += l;
    }
    cx->img[0] = 0;
    for (m = 1; m < N; m++) {
        int b = 0;
        uint32_t t = m;
        while (!(t & 1u)) { t >>= 1; b++; }
        cx->img[m] = cx->img[m & (m - 1)] | ((uint32_t)1 << cx->sig[b]);
    }
    memset(cx->vis, 0, N);
    for (m = 0; m < N; m++) {
        uint32_t x;
        uint64_t len = 0;
        if (cx->vis[m]) continue;
        x = m;
        while (!cx->vis[x]) { cx->vis[x] = 1; x = cx->img[x]; len++; }
        C++;
        if (len % 2 == 0) Ce++; else Co++;
        if (len % 3 == 0) C3++;
    }
    memcpy(cx->coef, cx->fact, (size_t)cx->CL * sizeof(uint32_t));
    i = 0;
    while (i < k) {
        int l = parts[i], mult = 0;
        while (i < k && parts[i] == l) {
            a871_divs(cx->coef, cx->CL, (uint32_t)l);
            mult++;
            i++;
        }
        for (j = 2; j <= mult; j++) a871_divs(cx->coef, cx->CL, (uint32_t)j);
    }
    a871_addsh(cx->acc, cx->L, cx->coef, cx->CL, 2 * C);
    for (j = 0; j < 3; j++) a871_addsh(cx->acc, cx->L, cx->coef, cx->CL, 2 * Ce + Co);
    for (j = 0; j < 2; j++) a871_addsh(cx->acc, cx->L, cx->coef, cx->CL, 2 * C3);
}

static void a871_part(a871_ctx *cx, int rem, int maxp, int *parts, int k) {
    int p;
    if (rem == 0) { a871_leaf(cx, parts, k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        parts[k] = p;
        a871_part(cx, rem - p, p, parts, k + 1);
    }
}

seq_t a(idx_t n) {
    a871_ctx cx;
    int nn, i, j, CL, L;
    uint32_t *fact;
    int *parts;
    seq_t r = 0;
    if (n < 1 || n > 24) return 0;
    nn = (int)n;
    CL = nn / 4 + 4;
    L = (int)((((uint64_t)1 << (nn + 1)) + 32 * (uint64_t)CL + 128) / 32) + 4;
    fact = (uint32_t *)calloc((size_t)CL, sizeof(uint32_t));
    cx.coef = (uint32_t *)calloc((size_t)CL, sizeof(uint32_t));
    cx.acc = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    cx.img = (uint32_t *)malloc(((size_t)1 << nn) * sizeof(uint32_t));
    cx.vis = (unsigned char *)malloc((size_t)1 << nn);
    cx.sig = (int *)malloc((size_t)(nn + 1) * sizeof(int));
    parts = (int *)malloc((size_t)(nn + 1) * sizeof(int));
    fact[0] = 1;
    for (i = 2; i <= nn; i++) a871_muls(fact, CL, (uint32_t)i);
    cx.n = nn;
    cx.L = L;
    cx.CL = CL;
    cx.fact = fact;
    a871_part(&cx, nn, nn, parts, 0);
    a871_divs(cx.acc, L, 6);
    for (i = 2; i <= nn; i++) a871_divs(cx.acc, L, (uint32_t)i);
    for (i = L - 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            r *= 256;
            r += (seq_t)((cx.acc[i] >> (8 * j)) & 0xffu);
        }
    }
    free(fact);
    free(cx.coef);
    free(cx.acc);
    free(cx.img);
    free(cx.vis);
    free(cx.sig);
    free(parts);
    return r;
}
