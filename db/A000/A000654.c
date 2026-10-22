/*
 * A000654  Invertible Boolean functions of n variables.
 * https://oeis.org/A000654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over (B_n x B_n) acting by f -> h.f.g^(-1):
 *            a(n) = (1/(2^n n!)^2) * Sum_{lambda} N(lambda)^2 * z(lambda),
 *            where lambda runs over cycle types of permutations of {0,1}^n,
 *            N(lambda) = number of signed permutations of the n variables
 *            inducing cycle type lambda on {0,1}^n, and z(lambd...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a654_gcd, a654_is_fixed, a654_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a654_gcd.h"
#include "../utils/a654_is_fixed.h"
#include "../utils/a654_mu.h"

typedef struct { int *v; int cnt, cap, w; } a654_plist;

static void a654_push(a654_plist *L, const int *cur)
{
    if (L->cnt == L->cap) {
        L->cap = L->cap ? 2 * L->cap : 16;
        L->v = (int *)realloc(L->v, (size_t)L->cap * (size_t)L->w * sizeof(int));
    }
    memcpy(L->v + (size_t)L->cnt * (size_t)L->w, cur, (size_t)L->w * sizeof(int));
    L->cnt++;
}

static void a654_gen(a654_plist *L, int rem, int maxk, int *cur)
{
    int k;
    if (rem == 0) { a654_push(L, cur); return; }
    for (k = rem < maxk ? rem : maxk; k >= 1; k--) {
        cur[k]++;
        a654_gen(L, rem - k, k, cur);
        cur[k]--;
    }
}

typedef struct { int nd; uint64_t *d; uint64_t *c; uint64_t N; } a654_type;

seq_t a(idx_t n)
{
    int N = (int)n, w = N + 1, m, i, j, k, t, ti;
    uint64_t G = 1;
    a654_plist *P;
    int *cur;
    a654_type *T = NULL;
    int tc = 0, tcap = 0;
    int fixed = a654_is_fixed();
    seq_t res = 0;

    if (fixed && N > 5) return 0; /* does not fit in any fixed width */

    for (i = 1; i <= N; i++) G *= (uint64_t)(2 * i);

    P = (a654_plist *)calloc((size_t)(N + 1), sizeof(a654_plist));
    cur = (int *)calloc((size_t)w, sizeof(int));
    for (m = 0; m <= N; m++) {
        P[m].w = w;
        a654_gen(&P[m], m, m, cur);
    }

    for (m = 0; m <= N; m++) {
        for (i = 0; i < P[m].cnt; i++) {
            for (j = 0; j < P[N - m].cnt; j++) {
                const int *al = P[m].v + (size_t)i * w;
                const int *be = P[N - m].v + (size_t)j * w;
                uint64_t cent = 1, size, Lc = 1, d, e;
                uint64_t *F, *dd, *cc;
                int nd = 0, found = -1;

                for (k = 1; k <= N; k++) {
                    for (t = 1; t <= al[k]; t++) cent *= (uint64_t)(2 * k) * (uint64_t)t;
                    for (t = 1; t <= be[k]; t++) cent *= (uint64_t)(2 * k) * (uint64_t)t;
                    if (al[k]) Lc = Lc / a654_gcd(Lc, (uint64_t)k) * (uint64_t)k;
                    if (be[k]) Lc = Lc / a654_gcd(Lc, (uint64_t)(2 * k)) * (uint64_t)(2 * k);
                }
                size = G / cent;

                F = (uint64_t *)calloc((size_t)(Lc + 1), sizeof(uint64_t));
                dd = (uint64_t *)malloc((size_t)(Lc + 1) * sizeof(uint64_t));
                cc = (uint64_t *)malloc((size_t)(Lc + 1) * sizeof(uint64_t));
                for (d = 1; d <= Lc; d++) {
                    int zero = 0;
                    uint64_t ex = 0;
                    if (Lc % d) continue;
                    for (k = 1; k <= N; k++) {
                        if (al[k]) ex += (uint64_t)al[k] * a654_gcd(d, (uint64_t)k);
                        if (be[k]) {
                            uint64_t g = a654_gcd(d, (uint64_t)k);
                            if ((d / g) % 2) zero = 1;
                            else ex += (uint64_t)be[k] * g;
                        }
                    }
                    F[d] = zero ? 0 : ((uint64_t)1 << ex);
                }
                for (d = 1; d <= Lc; d++) {
                    long long s = 0;
                    if (Lc % d) continue;
                    for (e = 1; e <= d; e++)
                        if (d % e == 0) s += (long long)a654_mu(d / e) * (long long)F[e];
                    s /= (long long)d;
                    if (s > 0) { dd[nd] = d; cc[nd] = (uint64_t)s; nd++; }
                }
                free(F);

                for (ti = 0; ti < tc && found < 0; ti++) {
                    if (T[ti].nd != nd) continue;
                    if (memcmp(T[ti].d, dd, (size_t)nd * sizeof(uint64_t)) == 0 &&
                        memcmp(T[ti].c, cc, (size_t)nd * sizeof(uint64_t)) == 0)
                        found = ti;
                }
                if (found >= 0) {
                    T[found].N += size;
                    free(dd);
                    free(cc);
                } else {
                    if (tc == tcap) {
                        tcap = tcap ? 2 * tcap : 64;
                        T = (a654_type *)realloc(T, (size_t)tcap * sizeof(a654_type));
                    }
                    T[tc].nd = nd;
                    T[tc].d = dd;
                    T[tc].c = cc;
                    T[tc].N = size;
                    tc++;
                }
            }
        }
    }

    if (fixed) {
        unsigned __int128 S = 0, r;
        for (ti = 0; ti < tc; ti++) {
            unsigned __int128 z = 1;
            uint64_t q;
            for (i = 0; i < T[ti].nd; i++)
                for (q = 1; q <= T[ti].c[i]; q++) {
                    z *= (unsigned __int128)T[ti].d[i];
                    z *= (unsigned __int128)q;
                }
            z *= (unsigned __int128)T[ti].N;
            z *= (unsigned __int128)T[ti].N;
            S += z;
        }
        r = S / G / G;
        res = (seq_t)(uint64_t)(r >> 64);
        res *= (seq_t)4294967296ULL;
        res *= (seq_t)4294967296ULL;
        res += (seq_t)(uint64_t)r;
    } else {
        seq_t S = 0;
        for (ti = 0; ti < tc; ti++) {
            seq_t z = 1;
            uint64_t q;
            for (i = 0; i < T[ti].nd; i++)
                for (q = 1; q <= T[ti].c[i]; q++) {
                    z *= (seq_t)T[ti].d[i];
                    z *= (seq_t)q;
                }
            z *= (seq_t)T[ti].N;
            z *= (seq_t)T[ti].N;
            S += z;
        }
        S /= (seq_t)G;
        S /= (seq_t)G;
        res = S;
    }

    for (ti = 0; ti < tc; ti++) { free(T[ti].d); free(T[ti].c); }
    free(T);
    for (m = 0; m <= N; m++) free(P[m].v);
    free(P);
    free(cur);
    return res;
}
