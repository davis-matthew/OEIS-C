/*
 * A003084  Related to number of digraphs.
 * https://oeis.org/A003084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sum a(n) x^n / n = log(1 + Sum d(n) x^n), d(n) = A000273, i.e.
 *            a(n) = n*d(n) - Sum_{k=1..n-1} a(k)*d(n-k), a(1) = 1, where d(n)
 *            = Sum_{partitions 1^m1 2^m2 ... of n} 2^e / Prod_p (p^mp * mp!),
 *            e = Sum_p [mp*(p-1) + mp*(mp-1)*p] + 2*Sum_{p<q} mp*mq*gcd(p,q).
 *            d(n) is computed exactly with a small i...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   x3084_div, x3084_gcd, x3084_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/x3084_div.h"
#include "../utils/x3084_gcd.h"
#include "../utils/x3084_mul.h"

#define X3084_MAXN 1000

static uint32_t *x3084_acc;
static uint32_t *x3084_buf;
static size_t x3084_alen, x3084_flen;
static idx_t *x3084_p;
static idx_t *x3084_m;

static void x3084_addshift(const uint32_t *t, size_t tl, uint64_t e) {
    size_t w = (size_t)(e / 32);
    unsigned bit = (unsigned)(e % 32);
    uint64_t carry = 0;
    size_t i;
    while (tl > 0 && t[tl - 1] == 0) tl--;
    for (i = 0; i < tl; i++) {
        uint64_t v = (uint64_t)t[i] << bit;
        uint64_t s = (uint64_t)x3084_acc[w + i] + (v & 0xffffffffULL) + carry;
        x3084_acc[w + i] = (uint32_t)s;
        carry = (s >> 32) + (v >> 32);
    }
    while (carry) {
        uint64_t s = (uint64_t)x3084_acc[w + i] + carry;
        x3084_acc[w + i] = (uint32_t)s;
        carry = s >> 32;
        i++;
    }
}

static void x3084_gen(idx_t rem, idx_t maxp, idx_t depth, uint64_t e) {
    uint32_t *cur = x3084_buf + (size_t)depth * x3084_flen;
    uint32_t *nxt = cur + x3084_flen;
    idx_t p, m, j;
    if (rem == 0) {
        x3084_addshift(cur, x3084_flen, e);
        return;
    }
    if (maxp > rem) maxp = rem;
    for (p = maxp; p >= 1; p--) {
        uint64_t g = 0;
        for (j = 0; j < depth; j++)
            g += (uint64_t)x3084_m[j] * x3084_gcd(p, x3084_p[j]);
        memcpy(nxt, cur, x3084_flen * sizeof(uint32_t));
        for (m = 1; m * p <= rem; m++) {
            uint64_t de = (uint64_t)m * (p - 1) + (uint64_t)m * (m - 1) * p
                          + 2 * (uint64_t)m * g;
            if (p > 1) x3084_div(nxt, x3084_flen, (uint32_t)p);
            if (m > 1) x3084_div(nxt, x3084_flen, (uint32_t)m);
            x3084_p[depth] = p;
            x3084_m[depth] = m;
            x3084_gen(rem - m * p, p - 1, depth + 1, e + de);
        }
    }
}

static seq_t x3084_digraphs(idx_t n) {
    size_t i;
    idx_t k;
    uint32_t *fact;
    seq_t r = (seq_t)0;
    /* n! needs at most n*log2(n)+1 bits */
    x3084_flen = 2;
    {
        double dummy = 0; (void)dummy;
    }
    for (k = 2; k <= n; k++) {
        idx_t b = 0, t = k;
        while (t) { b++; t >>= 1; }
        x3084_flen += 0; /* placeholder to keep loop simple */
        (void)b;
    }
    {
        uint64_t bits = 1;
        for (k = 2; k <= n; k++) {
            idx_t b = 0, t = k;
            while (t) { b++; t >>= 1; }
            bits += b;
        }
        x3084_flen = (size_t)(bits / 32 + 2);
    }
    x3084_alen = (size_t)(((uint64_t)n * (n - 1) + 32 * (uint64_t)x3084_flen) / 32 + 4);
    x3084_acc = (uint32_t *)calloc(x3084_alen, sizeof(uint32_t));
    x3084_buf = (uint32_t *)calloc((size_t)(n + 2) * x3084_flen, sizeof(uint32_t));
    x3084_p = (idx_t *)calloc((size_t)(n + 2), sizeof(idx_t));
    x3084_m = (idx_t *)calloc((size_t)(n + 2), sizeof(idx_t));
    fact = x3084_buf;
    fact[0] = 1;
    for (k = 2; k <= n; k++) x3084_mul(fact, x3084_flen, (uint32_t)k);
    x3084_gen(n, n, 0, 0);
    for (k = 2; k <= n; k++) x3084_div(x3084_acc, x3084_alen, (uint32_t)k);
    i = x3084_alen;
    while (i > 0 && x3084_acc[i - 1] == 0) i--;
    while (i-- > 0) {
        r = r * 65536u + (seq_t)(x3084_acc[i] >> 16);
        r = r * 65536u + (seq_t)(x3084_acc[i] & 0xffffu);
    }
    free(x3084_acc);
    free(x3084_buf);
    free(x3084_p);
    free(x3084_m);
    return r;
}

static seq_t x3084_A[X3084_MAXN + 1];
static seq_t x3084_D[X3084_MAXN + 1];
static idx_t x3084_cnt = 0;

seq_t a(idx_t n) {
    idx_t m, k;
    if (n < 1 || n > X3084_MAXN) return (seq_t)0;
    if (n <= x3084_cnt) return x3084_A[n];
    for (m = x3084_cnt + 1; m <= n; m++) {
        seq_t s;
        x3084_D[m] = x3084_digraphs(m);
        s = (seq_t)m * x3084_D[m];
        for (k = 1; k < m; k++) s = s - x3084_A[k] * x3084_D[m - k];
        x3084_A[m] = s;
    }
    x3084_cnt = n;
    return x3084_A[n];
}
