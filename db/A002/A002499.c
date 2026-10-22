/*
 * A002499  Number of self-converse digraphs with n nodes.
 * https://oeis.org/A002499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/n!) * Sum_{sigma in S_n} 2^c(sigma), where c(sigma) is
 *            the number of cycles of (i,j) -> (sigma(j),sigma(i)) on ordered
 *            pairs i != j (de Bruijn's count of digraphs isomorphic to their
 *            converse). For cycle type with parts a_i: c = Sum_i w(a_i) +
 *            Sum_{i<j} f(a_i,a_j), where w(a) = (a-1)/2...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   a2499_f, a2499_gcd, a2499_w
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2499_f.h"
#include "../utils/a2499_gcd.h"
#include "../utils/a2499_w.h"

typedef unsigned __int128 a2499_u128;

static int g2499_big;
static unsigned g2499_v;
static seq_t g2499_nfb, g2499_totb;
static a2499_u128 g2499_nff, g2499_low, g2499_high;
static idx_t *g2499_k, *g2499_m;

static void a2499_rec(idx_t rem, idx_t maxk, int d, uint64_t c, seq_t zb, a2499_u128 zf)
{
    idx_t k, m;
    int i;
    if (rem == 0) {
        if (g2499_big) {
            g2499_totb += (g2499_nfb / zb) << (idx_t)c;
        } else {
            a2499_u128 w;
            if (zf == 0) return;
            w = g2499_nff / zf;
            if (c < g2499_v) g2499_low += w << c;
            else if (c - g2499_v < 128) g2499_high += w << (c - g2499_v);
        }
        return;
    }
    for (k = (rem < maxk ? rem : maxk); k >= 1; k--) {
        uint64_t cross = 0, cc = c;
        seq_t zb2 = zb;
        a2499_u128 zf2 = zf;
        for (i = 0; i < d; i++) cross += (uint64_t)g2499_m[i] * a2499_f(k, g2499_k[i]);
        for (m = 1; m * k <= rem; m++) {
            cc += a2499_w(k) + (uint64_t)(m - 1) * a2499_f(k, k) + cross;
            if (g2499_big) { zb2 = zb2 * k; zb2 = zb2 * m; }
            else zf2 = zf2 * (a2499_u128)k * (a2499_u128)m;
            g2499_k[d] = k;
            g2499_m[d] = m;
            a2499_rec(rem - m * k, k - 1, d + 1, cc, zb2, zf2);
        }
    }
}

static seq_t a2499_conv(a2499_u128 v)
{
    seq_t r = 0;
    int i;
    for (i = 15; i >= 0; i--) {
        r = r * 256;
        r += (seq_t)(unsigned)((v >> (8 * i)) & 0xff);
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t t = 1;
    int i;
    idx_t j;
    for (i = 0; i < 200; i++) t = t + t;
    g2499_big = (t != 0);
    if (!g2499_big && n > 34) return 0;

    g2499_k = (idx_t *)malloc((n + 2) * sizeof(idx_t));
    g2499_m = (idx_t *)malloc((n + 2) * sizeof(idx_t));

    if (g2499_big) {
        g2499_nfb = 1;
        for (j = 2; j <= n; j++) g2499_nfb = g2499_nfb * j;
        g2499_totb = 0;
        a2499_rec(n, n, 0, 0, (seq_t)1, (a2499_u128)1);
        free(g2499_k);
        free(g2499_m);
        return g2499_totb / g2499_nfb;
    } else {
        a2499_u128 mo, y, X;
        idx_t p;
        g2499_nff = 1;
        for (j = 2; j <= n; j++) g2499_nff *= (a2499_u128)j;
        g2499_v = 0;
        for (p = 2; p <= n; p *= 2) g2499_v += (unsigned)(n / p);
        g2499_low = 0;
        g2499_high = 0;
        a2499_rec(n, n, 0, 0, (seq_t)1, (a2499_u128)1);
        free(g2499_k);
        free(g2499_m);
        X = (g2499_low >> g2499_v) + g2499_high;
        mo = g2499_nff >> g2499_v;
        y = mo;
        for (i = 0; i < 7; i++) y *= (a2499_u128)2 - mo * y;
        return a2499_conv(X * y);
    }
}
