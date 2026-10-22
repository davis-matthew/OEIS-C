/*
 * A003609  Symmetries in planted (1,3) trees on 2n vertices.
 * https://oeis.org/A003609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_T |Aut(T)| over planted (1,3) trees T on 2n vertices,
 *            i.e. over unordered binary rooted trees with n leaves. Let
 *            P_k(n) = Sum_T |Aut(T)|^k and, for n >= 2, G_k(n) = P_k(n)/2^k.
 *            Then G_k(2) = 1, and for n >= 3: G_k(n) = G_k(n-1) + 2^k *
 *            Sum_{2<=i<n-i} G_k(i)*G_k(n-i) + [n=2m, m>=2] ( 2^...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   A_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/A_grow.h"

#define A3609_LV 64

static seq_t *A3609_G[A3609_LV];
static idx_t A3609_cnt[A3609_LV];
static seq_t A3609_H[A3609_LV];  /* 2^(k-1), k = 2^e */
static seq_t A3609_T[A3609_LV];  /* 2^k */
static seq_t A3609_T2[A3609_LV]; /* 2^(2k) */
static int A3609_ready = 0;

static void A3609_init_levels(int e)
{
    while (A3609_ready <= e) {
        int l = A3609_ready;
        if (l == 0) {
            A3609_H[0] = 1;
        } else {
            A3609_H[l] = A3609_H[l - 1] * A3609_H[l - 1];
            A3609_H[l] = A3609_H[l] * 2;
        }
        A3609_T[l] = A3609_H[l] * 2;
        A3609_T2[l] = A3609_T[l] * A3609_T[l];
        A3609_ready++;
    }
}

static void A3609_ensure(int e, idx_t n)
{
    idx_t i, j;
    seq_t *g;
    if (n < A3609_cnt[e]) return;
    A3609_init_levels(e);
    if (n / 2 >= 2) A3609_ensure(e + 1, n / 2);
    A3609_G[e] = A_grow(A3609_G[e], A3609_cnt[e], n + 1);
    g = A3609_G[e];
    for (i = A3609_cnt[e]; i <= n; i++) {
        if (i < 2) {
            g[i] = 0;
        } else if (i == 2) {
            g[i] = 1;
        } else {
            seq_t s = g[i - 1];
            seq_t c = 0;
            for (j = 2; 2 * j < i; j++) {
                c += g[j] * g[i - j];
            }
            s += A3609_T[e] * c;
            if (i % 2 == 0 && i / 2 >= 2) {
                idx_t m = i / 2;
                seq_t gm = g[m];
                seq_t g2 = A3609_G[e + 1][m];
                s += A3609_H[e] * (gm * gm - g2);
                s += A3609_T2[e] * g2;
            }
            g[i] = s;
        }
    }
    A3609_cnt[e] = n + 1;
}

seq_t a(idx_t n)
{
    seq_t r;
    if (n <= 1) {
        r = 1;
        return r;
    }
    A3609_ensure(0, n);
    r = A3609_G[0][n] * 2;
    return r;
}
