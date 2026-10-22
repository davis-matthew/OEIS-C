/*
 * A000591  Number of n-state 2-input 1-output automata with one initial and
 *          one terminal state.
 * https://oeis.org/A000591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2*N!)) * Sum_{pi in S_N} f(pi)*(f(pi)-1) * (F(pi)^2 +
 *            F(pi^2)), with N = n+1, f(pi) = number of fixed points of pi,
 *            and F(pi) = Prod_{cycles of pi, length L} (Sum_{d|L} d*c_d(pi))
 *            = number of maps commuting with pi (c_d = number of d-cycles).
 *            The sum runs over cycle types, is computed...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Helpers:   a591_isprime, a591_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a591_isprime.h"
#include "../utils/a591_pw.h"

static int a591_K;
static long long a591_N;
static uint64_t *a591_P;
static uint64_t *a591_tot;
static uint64_t *a591_fac;
static long long a591_pl[512], a591_pc[512];

static void a591_leaf(int dep)
{
    long long ml[512], mc[512], B1[512], B2[512];
    int q = 0, i, t, j;
    long long c1 = a591_pc[0];
    for (i = 0; i < dep; i++) {
        long long L = a591_pl[i], c = a591_pc[i], Lm, cm;
        if (L % 2) { Lm = L; cm = c; } else { Lm = L / 2; cm = 2 * c; }
        for (t = 0; t < q; t++)
            if (ml[t] == Lm) break;
        if (t == q) { ml[q] = Lm; mc[q] = 0; q++; }
        mc[t] += cm;
    }
    for (i = 0; i < dep; i++) {
        B1[i] = 0;
        for (t = 0; t < dep; t++)
            if (a591_pl[i] % a591_pl[t] == 0) B1[i] += a591_pl[t] * a591_pc[t];
    }
    for (i = 0; i < q; i++) {
        B2[i] = 0;
        for (t = 0; t < q; t++)
            if (ml[i] % ml[t] == 0) B2[i] += ml[t] * mc[t];
    }
    for (j = 0; j < a591_K; j++) {
        uint64_t p = a591_P[j];
        uint64_t *fac = a591_fac + (size_t)j * (size_t)(a591_N + 1);
        uint64_t f = 1, g = 1, den, val;
        den = fac[c1 - 2];
        for (i = 0; i < dep; i++) {
            f = f * a591_pw((uint64_t)B1[i] % p, (uint64_t)a591_pc[i], p) % p;
            if (i > 0)
                den = den * a591_pw((uint64_t)a591_pl[i] % p, (uint64_t)a591_pc[i], p) % p
                      * fac[a591_pc[i]] % p;
        }
        for (i = 0; i < q; i++)
            g = g * a591_pw((uint64_t)B2[i] % p, (uint64_t)mc[i], p) % p;
        val = (f * f % p + g) % p;
        a591_tot[j] = (a591_tot[j] + val * a591_pw(den, p - 2, p)) % p;
    }
}

static void a591_dfs(int dep, long long last, long long r)
{
    long long L, k;
    if (r == 0) { a591_leaf(dep); return; }
    for (L = last + 1; L <= r; L++) {
        a591_pl[dep] = L;
        for (k = 1; k * L <= r; k++) {
            a591_pc[dep] = k;
            a591_dfs(dep + 1, L, r - k * L);
        }
    }
}

seq_t a(idx_t n)
{
    long long N = (long long)n + 1, c1, i;
    int bl = 0, j, jj, x;
    uint64_t c;
    uint64_t *v;
    seq_t res;

    a591_N = N;
    while ((N >> bl) != 0) bl++;
    a591_K = (int)((long long)bl * (2 * N + 4) / 30 + 2);
    a591_P = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)a591_K);
    c = 2147483647ULL;
    for (j = 0; j < a591_K; c -= 2) {
        if (a591_isprime(c)) a591_P[j++] = c;
    }
    a591_fac = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)a591_K * (size_t)(N + 1));
    for (j = 0; j < a591_K; j++) {
        uint64_t p = a591_P[j];
        uint64_t *fac = a591_fac + (size_t)j * (size_t)(N + 1);
        fac[0] = 1;
        for (i = 1; i <= N; i++) fac[i] = fac[i - 1] * (uint64_t)i % p;
    }
    a591_tot = (uint64_t *)calloc((size_t)a591_K, sizeof(uint64_t));

    a591_pl[0] = 1;
    for (c1 = 2; c1 <= N; c1++) {
        a591_pc[0] = c1;
        a591_dfs(1, 1, N - c1);
    }

    v = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)a591_K);
    for (j = 0; j < a591_K; j++) {
        uint64_t p = a591_P[j];
        uint64_t t = a591_tot[j] * a591_pw(2, p - 2, p) % p;
        for (jj = 0; jj < j; jj++) {
            t = (t + p - v[jj] % p) % p;
            t = t * a591_pw(a591_P[jj] % p, p - 2, p) % p;
        }
        v[j] = t;
    }
    res = 0;
    for (x = a591_K - 1; x >= 0; x--)
        res = res * (seq_t)a591_P[x] + (seq_t)v[x];

    free(v);
    free(a591_P);
    free(a591_fac);
    free(a591_tot);
    return res;
}
