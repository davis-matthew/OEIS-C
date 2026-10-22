/*
 * A001173  Half the number of binary relations on n unlabeled points.
 * https://oeis.org/A001173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A000595(n)/2, where A000595(n) = Sum_{partitions lambda
 *            of n} 2^(Sum_{i,j} gcd(lambda_i,lambda_j)) / z(lambda), and
 *            z(lambda) = Prod_k k^m_k * m_k!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   a1173_divsmall, a1173_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/a1173_divsmall.h"
#include "../utils/a1173_gcd.h"

static uint32_t *g_acc;
static size_t g_accL;
static uint32_t *g_cb;
static size_t g_L;
static idx_t *g_sp;
static idx_t *g_sm;

static void a1173_addsh(const uint32_t *c, size_t len, uint64_t e) {
    size_t w = (size_t)(e / 32);
    unsigned s = (unsigned)(e % 32);
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t x = ((uint64_t)c[i]) << s;
        uint64_t t = (uint64_t)g_acc[w + i] + (x & 0xffffffffULL) + carry;
        g_acc[w + i] = (uint32_t)t;
        carry = (t >> 32) + (x >> 32);
    }
    i = w + len;
    while (carry && i < g_accL) {
        uint64_t t = (uint64_t)g_acc[i] + carry;
        g_acc[i] = (uint32_t)t;
        carry = t >> 32;
        i++;
    }
}

static void a1173_rec(idx_t k, idx_t rem, const uint32_t *c, size_t clen,
                      uint64_t e, idx_t depth) {
    if (rem == 0) { a1173_addsh(c, clen, e); return; }
    if (k > rem) k = rem;
    uint32_t *b = g_cb + (size_t)k * g_L;
    memcpy(b, c, clen * sizeof(uint32_t));
    size_t bl = clen;
    uint64_t G = 0;
    idx_t d, m;
    for (d = 0; d < depth; d++) G += (uint64_t)g_sm[d] * a1173_gcd(k, g_sp[d]);
    for (m = 0; m * k <= rem; m++) {
        if (m > 0) a1173_divsmall(b, &bl, (uint64_t)k * m);
        if (k == 1 && m < rem) continue;
        g_sp[depth] = k;
        g_sm[depth] = m;
        uint64_t em = e + (uint64_t)m * m * k + 2 * (uint64_t)m * G;
        a1173_rec(k - 1, rem - m * k, b, bl, em, m ? depth + 1 : depth);
    }
}

seq_t a(idx_t n) {
    idx_t i;
    unsigned bits = 0;
    idx_t t = n;
    while (t) { bits++; t >>= 1; }
    g_L = (size_t)((n * bits) / 32 + 3);
    g_accL = (size_t)(n * n / 32) + g_L + 4;
    g_acc = (uint32_t *)calloc(g_accL, sizeof(uint32_t));
    g_cb = (uint32_t *)calloc((size_t)(n + 2) * g_L, sizeof(uint32_t));
    g_sp = (idx_t *)calloc(n + 2, sizeof(idx_t));
    g_sm = (idx_t *)calloc(n + 2, sizeof(idx_t));
    uint32_t *f = (uint32_t *)calloc(g_L, sizeof(uint32_t));
    size_t fl = 1;
    f[0] = 1;
    for (i = 2; i <= n; i++) {
        uint64_t carry = 0;
        size_t j;
        for (j = 0; j < fl; j++) {
            uint64_t v = (uint64_t)f[j] * i + carry;
            f[j] = (uint32_t)v;
            carry = v >> 32;
        }
        while (carry) { f[fl++] = (uint32_t)carry; carry >>= 32; }
    }
    a1173_rec(n, n, f, fl, 0, 0);
    size_t al = g_accL;
    while (al > 0 && g_acc[al - 1] == 0) al--;
    a1173_divsmall(g_acc, &al, 2);
    for (i = 2; i <= n; i++) a1173_divsmall(g_acc, &al, i);
    seq_t r = 0;
    size_t j;
    for (j = al; j-- > 0;) {
        r = (r << 16) + (seq_t)(g_acc[j] >> 16);
        r = (r << 16) + (seq_t)(g_acc[j] & 0xffffu);
    }
    free(f);
    free(g_sm);
    free(g_sp);
    free(g_cb);
    free(g_acc);
    return r;
}
