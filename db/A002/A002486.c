/*
 * A002486  Apart from two leading terms (which are present by convention),
 *          denominators of convergents to Pi (A002485 and A046947 give
 *          numerators).
 * https://oeis.org/A002486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, frac
 * Formula:   a(0) = 1, a(1) = 0, a(n) = c(n-2)*a(n-1) + a(n-2) for n >= 2,
 *            where c(k) = A001203(k+1) are the continued fraction terms of Pi
 *            = [3; 7, 15, 1, 292, ...]. Pi is computed by Machin's formula Pi
 *            = 16*atan(1/5) - 4*atan(1/239) in fixed point, and its CF terms
 *            are taken from the common prefix of the C...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 20, u64 n <= 34, u128 n <= 78,
 *            big any n
 * Helpers:   pc_bits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pc_bits.h"

static uint64_t *pcf_terms = NULL;
static size_t pcf_cnt = 0;

static void pc_divs(uint32_t *x, size_t len, uint32_t d)
{
    uint64_t r = 0;
    size_t i = len;
    while (i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void pc_add(uint32_t *a, const uint32_t *b, size_t n, size_t W)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t s = (uint64_t)a[i] + b[i] + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
    }
    for (; c && i < W; i++) {
        uint64_t s = (uint64_t)a[i] + c;
        a[i] = (uint32_t)s;
        c = s >> 32;
    }
}

static void pc_sub(uint32_t *a, const uint32_t *b, size_t n, size_t W)
{
    uint64_t br = 0;
    size_t i;
    for (i = 0; i < n; i++) {
        uint64_t s = (uint64_t)a[i] - b[i] - br;
        a[i] = (uint32_t)s;
        br = (s >> 63) & 1;
    }
    for (; br && i < W; i++) {
        uint64_t s = (uint64_t)a[i] - br;
        a[i] = (uint32_t)s;
        br = (s >> 63) & 1;
    }
}

static void pc_muls(uint32_t *x, size_t W, uint32_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t p = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)p;
        c = p >> 32;
    }
}

static void pc_atan_inv(uint32_t *sum, size_t W, size_t F, uint32_t x, uint32_t *t, uint32_t *u)
{
    uint32_t xx = x * x;
    size_t top;
    uint64_t k;
    memset(t, 0, W * sizeof(uint32_t));
    t[F] = 1;
    pc_divs(t, W, x);
    memcpy(sum, t, W * sizeof(uint32_t));
    top = W;
    while (top > 0 && t[top - 1] == 0) top--;
    for (k = 1;; k++) {
        pc_divs(t, top, xx);
        while (top > 0 && t[top - 1] == 0) top--;
        if (top == 0) break;
        memcpy(u, t, top * sizeof(uint32_t));
        pc_divs(u, top, (uint32_t)(2 * k + 1));
        if (k & 1) pc_sub(sum, u, top, W);
        else pc_add(sum, u, top, W);
    }
}

static size_t pc_bitlen(const uint32_t *x, size_t len)
{
    uint32_t v;
    size_t b;
    if (len == 0) return 0;
    v = x[len - 1];
    b = 0;
    while (v) { b++; v >>= 1; }
    return (len - 1) * 32 + b;
}

static int pc_cmp(const uint32_t *a, size_t al, const uint32_t *b, size_t bl)
{
    size_t i;
    if (al != bl) return al < bl ? -1 : 1;
    i = al;
    while (i--) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void pc_submul(uint32_t *a, size_t *al, const uint32_t *b, size_t bl, uint32_t q)
{
    uint64_t c = 0, br = 0;
    size_t i;
    for (i = 0; i < bl; i++) {
        uint64_t p = (uint64_t)q * b[i] + c;
        uint64_t s;
        c = p >> 32;
        s = (uint64_t)a[i] - (uint32_t)p - br;
        a[i] = (uint32_t)s;
        br = (s >> 63) & 1;
    }
    for (; i < *al && (c || br); i++) {
        uint64_t s = (uint64_t)a[i] - c - br;
        c = 0;
        a[i] = (uint32_t)s;
        br = (s >> 63) & 1;
    }
    while (*al > 0 && a[*al - 1] == 0) (*al)--;
}

static size_t pc_cf_run(uint32_t *a, size_t al, uint32_t *b, size_t bl, size_t cap, uint64_t *out)
{
    size_t cnt = 0;
    while (al > 0 && a[al - 1] == 0) al--;
    while (bl > 0 && b[bl - 1] == 0) bl--;
    while (bl > 0 && cnt < cap) {
        uint64_t c = 0;
        uint32_t *tp;
        size_t tl;
        while (pc_cmp(a, al, b, bl) >= 0) {
            size_t s = pc_bitlen(a, al);
            size_t sh = s > 64 ? s - 64 : 0;
            uint64_t ah = pc_bits(a, al, sh);
            uint64_t bh = pc_bits(b, bl, sh);
            uint64_t q;
            if (bh == UINT64_MAX) q = 1;
            else q = ah / (bh + 1);
            if (q > 0xFFFFFFFFull) q = 0xFFFFFFFFull;
            if (q == 0) q = 1;
            pc_submul(a, &al, b, bl, (uint32_t)q);
            c += q;
        }
        out[cnt++] = c;
        tp = a; a = b; b = tp;
        tl = al; al = bl; bl = tl;
    }
    return cnt;
}

static void pc_compute(size_t m)
{
    for (;;) {
        size_t bits = 4 * m + 160;
        size_t F = bits / 32 + 3;
        size_t W = F + 1;
        size_t PL = F - 1, i, k1, k2, j;
        uint32_t *s5 = (uint32_t *)calloc(W, sizeof(uint32_t));
        uint32_t *s239 = (uint32_t *)calloc(W, sizeof(uint32_t));
        uint32_t *t = (uint32_t *)calloc(W, sizeof(uint32_t));
        uint32_t *u = (uint32_t *)calloc(W, sizeof(uint32_t));
        uint32_t *lo = (uint32_t *)calloc(PL + 1, sizeof(uint32_t));
        uint32_t *hi = (uint32_t *)calloc(PL + 1, sizeof(uint32_t));
        uint32_t *d1 = (uint32_t *)calloc(PL + 1, sizeof(uint32_t));
        uint32_t *d2 = (uint32_t *)calloc(PL + 1, sizeof(uint32_t));
        uint64_t *o1 = (uint64_t *)malloc((m + 2) * sizeof(uint64_t));
        uint64_t *o2 = (uint64_t *)malloc((m + 2) * sizeof(uint64_t));
        uint32_t two[1];

        pc_atan_inv(s5, W, F, 5, t, u);
        pc_atan_inv(s239, W, F, 239, t, u);
        pc_muls(s5, W, 16);
        pc_muls(s239, W, 4);
        pc_sub(s5, s239, W, W);

        for (i = 0; i < PL; i++) { lo[i] = s5[i + 2]; hi[i] = s5[i + 2]; }
        two[0] = 2;
        pc_sub(lo, two, 1, PL + 1);
        pc_add(hi, two, 1, PL + 1);
        d1[F - 2] = 1;
        d2[F - 2] = 1;

        k1 = pc_cf_run(lo, PL + 1, d1, PL + 1, m + 2, o1);
        k2 = pc_cf_run(hi, PL + 1, d2, PL + 1, m + 2, o2);
        j = 0;
        while (j < k1 && j < k2 && o1[j] == o2[j]) j++;
        if (j == k1 || j == k2) { if (j > 0) j--; }

        free(s5); free(s239); free(t); free(u);
        free(lo); free(hi); free(d1); free(d2); free(o2);

        if (j >= m) {
            free(pcf_terms);
            pcf_terms = o1;
            pcf_cnt = j;
            return;
        }
        free(o1);
        m *= 2;
    }
}

seq_t a(idx_t n)
{
    seq_t p2, p1;
    idx_t i;
    if (n == 0) return 1;
    if (n == 1) return 0;
    if ((size_t)(n - 1) > pcf_cnt) {
        size_t m = (size_t)(n - 1);
        if (m < 2 * pcf_cnt) m = 2 * pcf_cnt;
        if (m < 256) m = 256;
        pc_compute(m);
    }
    p2 = 1;
    p1 = 0;
    for (i = 0; i + 2 <= n; i++) {
        seq_t q = (seq_t)pcf_terms[i] * p1 + p2;
        p2 = p1;
        p1 = q;
    }
    return p1;
}
