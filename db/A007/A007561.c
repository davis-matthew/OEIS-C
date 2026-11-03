/*
 * A007561  Number of asymmetric rooted connected graphs where every block is
 *          a complete graph.
 * https://oeis.org/A007561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n+1) = WEIGH(WEIGH(a))(n), a(0) = 0, a(1) = 1, where
 *            WEIGH(u)(n) = [x^n] Prod_{k>=1} (1+x^k)^u(k). Arbitrary
 *            precision: n*w(n) = Sum_{k=1..n} e(k)*w(n-k), e(k) = Sum_{d|k}
 *            (-1)^(k/d+1)*d*u(d). Fixed width: the products are expanded
 *            directly with exact binomials
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 42, u128 n <= 79,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 1775 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static void ecomp(seq_t *u, idx_t m, seq_t *P, seq_t *Ng)
{
    idx_t d;
    seq_t p = 0, q = 0;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            idx_t e = m / d;
            seq_t v = (seq_t)d * u[d];
            if (e & 1) p += v; else q += v;
            if (e != d) {
                v = (seq_t)e * u[e];
                if (d & 1) p += v; else q += v;
            }
        }
    }
    P[m] = p; Ng[m] = q;
}

static seq_t wstep(seq_t *w, seq_t *P, seq_t *Ng, idx_t m)
{
    seq_t sp = 0, sn = 0;
    idx_t k;
    for (k = 1; k <= m; k++) {
        if (P[k] != 0) sp += P[k] * w[m - k];
        if (Ng[k] != 0) sn += Ng[k] * w[m - k];
    }
    return (sp - sn) / (seq_t)m;
}

/* P(x) *= (1+x^k)^m, truncated at degree N */
static void mulfac(seq_t *P, idx_t N, idx_t k, seq_t m, seq_t *bin)
{
    idx_t J = N / k, j, i;
    bool zero = false;
    bin[0] = 1;
    for (j = 1; j <= J; j++) {
        if (zero || m < (seq_t)j) { zero = true; bin[j] = 0; continue; }
        {
            seq_t C = bin[j - 1];
            seq_t g = gcd(C, (seq_t)j);
            seq_t q = (seq_t)j / g;
            bin[j] = (C / g) * ((m - (seq_t)j + 1) / q);
        }
    }
    for (i = N; i >= k; i--) {
        seq_t s = 0;
        for (j = 1; j * k <= i; j++)
            if (bin[j] != 0) s += bin[j] * P[i - j * k];
        P[i] += s;
        if (i == k) break;
    }
}

static seq_t small_a(idx_t n)
{
    idx_t N, m;
    seq_t *B, *C, *bin, r;
    if (n == 0) return 0;
    N = n - 1;
    B = alloc_zero(N + 1);
    C = alloc_zero(N + 1);
    bin = alloc_zero(N + 2);
    B[0] = 1; C[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t am = C[m - 1];
        mulfac(B, N, m, am, bin);
        mulfac(C, N, m, B[m], bin);
    }
    r = C[N];
    release(B, N + 1); release(C, N + 1); release(bin, N + 2);
    return r;
}

static seq_t *A, *Bv, *Cv, *AP, *AN, *BP, *BN;
static idx_t cnt = 0, cap = 0;

seq_t a(idx_t n)
{
    idx_t m;
    if (seq_is_fixed_width() && n <= 200) return small_a(n);
    if (n == 0) return 0;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        A = grow_zero(A, cap, nc);
        Bv = grow_zero(Bv, cap, nc);
        Cv = grow_zero(Cv, cap, nc);
        AP = grow_zero(AP, cap, nc);
        AN = grow_zero(AN, cap, nc);
        BP = grow_zero(BP, cap, nc);
        BN = grow_zero(BN, cap, nc);
        cap = nc;
    }
    if (cnt == 0) {
        A[0] = 0; Bv[0] = 1; Cv[0] = 1;
        AP[0] = 0; AN[0] = 0; BP[0] = 0; BN[0] = 0;
        cnt = 1;
    }
    for (m = cnt; m <= n; m++) {
        A[m] = Cv[m - 1];
        ecomp(A, m, AP, AN);
        Bv[m] = wstep(Bv, AP, AN, m);
        ecomp(Bv, m, BP, BN);
        Cv[m] = wstep(Cv, BP, BN, m);
        cnt = m + 1;
    }
    return A[n];
}
