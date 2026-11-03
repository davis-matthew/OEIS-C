/*
 * A005797  Expansion of Jacobi nome q in terms of parameter m/16.
 * https://oeis.org/A005797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   with g(n) = A036917(n), g(0) = 1, n^3*g(n) =
 *            8*(2n-1)*(2n^2-2n+1)*g(n-1) - 256*(n-1)^3*g(n-2), and d(k) =
 *            16*g(k-1) - g(k), the Wronskian identity x*q'/q =
 *            1/((1-16x)*F(x)^2) with F(x)^2 = Sum g(n)*x^n gives a(n) = (
 *            Sum_{i=1..n-1} i*a(i)*d(n-i) ) / (n-1), a(0) = 0, a(1) = 1.
 *            Fixed-width builds i...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 696 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
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

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

typedef unsigned __int128 u128_t;

static seq_t *A_ = NULL, *G_ = NULL, *D_ = NULL;
static idx_t cap_ = 0, done_ = 0;

static void extend_big(idx_t n)
{
    idx_t k, i;
    if (n + 1 > cap_) {
        idx_t nc = cap_ ? cap_ : 16;
        while (nc < n + 1) nc *= 2;
        if (cap_ == 0) {
            A_ = alloc_zero(nc);
            G_ = alloc_zero(nc);
            D_ = alloc_zero(nc);
        } else {
            A_ = grow_zero(A_, cap_, nc);
            G_ = grow_zero(G_, cap_, nc);
            D_ = grow_zero(D_, cap_, nc);
        }
        cap_ = nc;
    }
    for (k = done_; k <= n; k++) {
        if (k == 0) {
            G_[0] = 1;
            A_[0] = 0;
            continue;
        }
        {
            seq_t t = (seq_t)(8 * (2 * k - 1) * (2 * k * k - 2 * k + 1)) * G_[k - 1];
            if (k >= 2)
                t -= (seq_t)(256 * (k - 1) * (k - 1) * (k - 1)) * G_[k - 2];
            G_[k] = t / ((seq_t)k * k * k);
        }
        D_[k] = G_[k - 1] * 16 - G_[k];
        if (k == 1) {
            A_[1] = 1;
        } else {
            seq_t s = 0;
            for (i = 1; i < k; i++)
                s += (seq_t)i * A_[i] * D_[k - i];
            A_[k] = s / (seq_t)(k - 1);
        }
    }
    if (n + 1 > done_) done_ = n + 1;
}

#define FN 128
static u128_t *FT_ = NULL;

static void build_fixed(void)
{
    idx_t W = FN + 1;
    u128_t *e = (u128_t *)calloc(W, sizeof(u128_t));
    u128_t *P = (u128_t *)calloc(W * W, sizeof(u128_t));
    u128_t *ft = (u128_t *)calloc(W, sizeof(u128_t));
    idx_t j, r, i, m, k;
    e[0] = 1;
    for (j = 1; j <= FN; j++) {
        for (r = 0; r < 8; r++) {
            if (j % 2 == 0) {
                for (i = FN; i >= j; i--)
                    e[i] += e[i - j];
            } else {
                for (i = j; i <= FN; i++)
                    e[i] -= e[i - j];
            }
        }
    }
    ft[0] = 0;
    ft[1] = 1;
    P[1 * W + 1] = 1;
    for (m = 2; m <= FN; m++) {
        u128_t s = 0;
        for (k = 2; k <= m; k++) {
            u128_t t = 0;
            for (i = 1; i + k - 1 <= m; i++)
                t += ft[i] * P[(k - 1) * W + (m - i)];
            P[k * W + m] = t;
            s += e[k - 1] * t;
        }
        ft[m] = (u128_t)0 - s;
        P[1 * W + m] = ft[m];
    }
    free(e);
    free(P);
    FT_ = ft;
}

seq_t a(idx_t n)
{
    if (seq_is_fixed_width()) {
        if (n > FN) return 0;
        if (!FT_) build_fixed();
        return u128_to_seq(FT_[n]);
    }
    if (n >= done_) extend_big(n);
    return A_[n];
}
