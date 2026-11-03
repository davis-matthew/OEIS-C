/*
 * A007243  McKay-Thompson series of class 3A for the Monster group with a(0)
 *          = 0.
 * https://oeis.org/A007243
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   T3A + 42 = theta_A2(q)^6 / (eta(q)*eta(q^3))^6, where
 *            theta_A2(q) = 1 + 6*Sum_{k>=1} (Sum_{d|k} chi_3(d)) q^k (cubic
 *            theta a(q)), since T3A = (eta(q)/eta(q^3))^12 + 12 +
 *            729*(eta(q^3)/eta(q))^12. 1/(eta(q)eta(q^3))^6 is obtained by
 *            sparse division by Jacobi's eta(q)^3 = Sum_{m>=0} (-1)^m (2m+1)
 *            q...
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 11, u64 n <= 43, u128 n <= 164,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 9026 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

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

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
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

/* in-place division of series G (length len) by eta(q^step)^3 / q^(step/8) */
static void div_e3(seq_t *g, idx_t len, idx_t step)
{
    idx_t n, m, t;
    for (n = 1; n < len; n++) {
        seq_t pos = g[n], neg = 0;
        for (m = 1; (t = step * m * (m + 1) / 2) <= n; m++) {
            if (m & 1) pos += g[n - t] * (2 * m + 1);
            else neg += g[n - t] * (2 * m + 1);
        }
        g[n] = pos - neg;
    }
}

static idx_t cap = 0;
static seq_t *G = NULL;               /* coeffs of prod 1/((1-q^k)^6 (1-q^3k)^6) */
static unsigned __int128 *A6 = NULL;  /* coeffs of theta_A2^6 */
static seq_t *T = NULL;
static bool *Tok = NULL;

static void ensure(idx_t need)
{
    idx_t len, i, k, d;
    long long *th, *a2, *a3;
    if (need <= cap) return;
    len = 2 * cap;
    if (len < need) len = need;
    if (len < 16) len = 16;

    th = (long long *)calloc((size_t)len, sizeof(long long));
    a2 = (long long *)calloc((size_t)len, sizeof(long long));
    a3 = (long long *)calloc((size_t)len, sizeof(long long));
    th[0] = 1;
    for (d = 1; d < len; d++) {
        int chi = (d % 3 == 1) ? 1 : (d % 3 == 2) ? -1 : 0;
        if (chi == 0) continue;
        for (k = d; k < len; k += d) th[k] += 6 * chi;
    }
    for (k = 0; k < len; k++) {
        long long s = 0;
        for (i = 0; i <= k; i++) s += th[i] * th[k - i];
        a2[k] = s;
    }
    for (k = 0; k < len; k++) {
        long long s = 0;
        for (i = 0; i <= k; i++) s += a2[i] * th[k - i];
        a3[k] = s;
    }
    A6 = (unsigned __int128 *)realloc(A6, (size_t)len * sizeof(unsigned __int128));
    for (k = 0; k < len; k++) {
        unsigned __int128 s = 0;
        for (i = 0; i <= k; i++)
            s += (unsigned __int128)(unsigned long long)a3[i] * (unsigned long long)a3[k - i];
        A6[k] = s;
    }
    free(th);
    free(a2);
    free(a3);

    if (G == NULL) {
        G = alloc_zero(len);
        T = alloc_zero(len);
    } else {
        G = grow_zero(G, cap, len);
        T = grow_zero(T, cap, len);
    }
    Tok = (bool *)realloc(Tok, (size_t)len * sizeof(bool));
    for (i = cap; i < len; i++) Tok[i] = false;

    for (i = 0; i < len; i++) G[i] = 0;
    G[0] = 1;
    div_e3(G, len, 1);
    div_e3(G, len, 1);
    div_e3(G, len, 3);
    div_e3(G, len, 3);

    cap = len;
}

seq_t a(idx_t n)
{
    idx_t i;
    seq_t s;
    if (n == -1) return 1;
    if (n == 0) return 0;
    ensure(n + 2);
    if (Tok[n]) return T[n];
    s = 0;
    for (i = 0; i <= n + 1; i++)
        s += u128_to_seq(A6[i]) * G[n + 1 - i];
    T[n] = s;
    Tok[n] = true;
    return s;
}
