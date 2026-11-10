/*
 * A055152  Proper covers of an unlabeled n-set.
 * https://oeis.org/A055152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A002857(n) - A000612(n-1), with a(1) = 0. Here A000612(n)
 *            = Sum_{partitions L of n} 2^(cyc(L)-1)/z_L, A002857(n) = Sum_{L}
 *            2^cyc(L)/(4*z_L), and cyc(L) is the number of cycles of a
 *            permutation of cycle type L on {0,1}^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 12 terms through n = 12 (2026-11-07)
 * Helpers:   gcd_u64, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/totient_u64.h"

static int a2857_is_fixed(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 8; i++) t = t << 16;
    return t == 0;
}

typedef unsigned __int128 a2857_u128;

static uint32_t *a2857_num;
static size_t a2857_len;
static int a2857_parts[128];
static a2857_u128 a2857_nfact;

static void a2857_add(a2857_u128 v, uint64_t pos)
{
    size_t idx = (size_t)(pos / 32);
    unsigned s = (unsigned)(pos % 32);
    uint32_t lim[4];
    uint64_t w[5], carry = 0;
    int i;
    for (i = 0; i < 4; i++) lim[i] = (uint32_t)(v >> (32 * i));
    for (i = 0; i < 5; i++) {
        uint64_t cur = 0;
        if (i < 4) cur = ((uint64_t)lim[i] << s) & 0xffffffffULL;
        if (i > 0) cur |= ((uint64_t)lim[i - 1] << s) >> 32;
        w[i] = cur;
    }
    for (i = 0; i < 5 || carry; i++) {
        uint64_t t = (uint64_t)a2857_num[idx + i] + (i < 5 ? w[i] : 0) + carry;
        a2857_num[idx + i] = (uint32_t)t;
        carry = t >> 32;
    }
}

static void a2857_divsmall(uint32_t d)
{
    uint64_t rem = 0;
    size_t i = a2857_len;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (rem << 32) | a2857_num[i];
        a2857_num[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void a2857_process(int k)
{
    uint64_t m = 1, sum = 0, d, cyc;
    a2857_u128 z = 1;
    int i, run = 0;
    for (i = 0; i < k; i++) {
        uint64_t p = (uint64_t)a2857_parts[i];
        m = m / gcd_u64(m, p) * p;
        if (i > 0 && a2857_parts[i] == a2857_parts[i - 1]) run++;
        else run = 1;
        z *= (a2857_u128)p;
        z *= (a2857_u128)run;
    }
    for (d = 1; d <= m; d++) {
        if (m % d == 0) {
            uint64_t s = 0;
            for (i = 0; i < k; i++) s += gcd_u64((uint64_t)a2857_parts[i], d);
            sum += totient_u64(m / d) * ((uint64_t)1 << s);
        }
    }
    cyc = sum / m;
    a2857_add(a2857_nfact / z, cyc);
}

static void a2857_gen(int rem, int maxp, int k)
{
    int p;
    if (rem == 0) { a2857_process(k); return; }
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        a2857_parts[k] = p;
        a2857_gen(rem - p, p, k + 1);
    }
}

static seq_t a2857_conv(size_t lo, size_t len)
{
    seq_t r = 0;
    if (len == 1) {
        r += (idx_t)a2857_num[lo];
        return r;
    } else {
        size_t h = len / 2;
        r = a2857_conv(lo + h, len - h);
        r <<= (idx_t)(32 * h);
        r += a2857_conv(lo, h);
        return r;
    }
}

static seq_t A002857_a(idx_t n)
{
    seq_t r = 0;
    uint64_t nbits;
    idx_t k;
    if (n < 1) return r;
    nbits = (uint64_t)1 << n;
    a2857_len = (size_t)(nbits / 32 + 12);
    a2857_num = (uint32_t *)calloc(a2857_len, sizeof(uint32_t));
    a2857_nfact = 1;
    for (k = 2; k <= n; k++) a2857_nfact *= (a2857_u128)k;

    a2857_gen((int)n, (int)n, 0);

    a2857_divsmall(4);
    for (k = 2; k <= n; k++) a2857_divsmall((uint32_t)k);

    if (a2857_is_fixed()) {
        int i, b;
        for (i = 3; i >= 0; i--) {
            uint32_t limb = (size_t)i < a2857_len ? a2857_num[i] : 0;
            for (b = 3; b >= 0; b--) {
                r = r << 8;
                r += (idx_t)((limb >> (8 * b)) & 255u);
            }
        }
    } else {
        size_t len = a2857_len;
        while (len > 1 && a2857_num[len - 1] == 0) len--;
        r = a2857_conv(0, len);
    }
    free(a2857_num);
    a2857_num = NULL;
    return r;
}

static void h612_rec(int rem, int maxp, int *m, int n, seq_t nf, seq_t *total) {
    if (rem == 0) {
        uint64_t L = 1;
        int i;
        for (i = 1; i <= n; i++)
            if (m[i]) L = L / gcd_u64(L, (uint64_t)i) * (uint64_t)i;
        unsigned __int128 sum = 0;
        uint64_t k;
        for (k = 0; k < L; k++) {
            uint64_t cyc = 0;
            for (i = 1; i <= n; i++)
                if (m[i]) cyc += (uint64_t)m[i] * gcd_u64((uint64_t)i, k);
            sum += (unsigned __int128)1 << cyc;
        }
        uint64_t c = (uint64_t)(sum / L);
        seq_t z = 1;
        for (i = 1; i <= n; i++) {
            int j;
            for (j = 1; j <= m[i]; j++) {
                z *= (seq_t)i;
                z *= (seq_t)j;
            }
        }
        seq_t term = nf / z;
        term <<= (int)(c - 1);
        *total += term;
        return;
    }
    int p;
    for (p = (rem < maxp ? rem : maxp); p >= 1; p--) {
        m[p]++;
        h612_rec(rem - p, p, m, n, nf, total);
        m[p]--;
    }
}

static seq_t A000612_a(idx_t n) {
    seq_t x = 1;
    int w = 0;
    while (x != 0 && w < 300) { x <<= 1; w++; }
    if (w < 300) {
        if (n >= 8 || ((uint64_t)1 << n) > (uint64_t)w) return 0;
    }
    int nn = (int)n;
    int *m = (int *)calloc((size_t)nn + 2, sizeof(int));
    seq_t nf = 1;
    int i;
    for (i = 2; i <= nn; i++) nf *= (seq_t)i;
    seq_t total = 0;
    h612_rec(nn, nn, m, nn, nf, &total);
    free(m);
    return total / nf;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    return A002857_a(n) - A000612_a(n - 1);
}
