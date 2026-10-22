/*
 * A005641  Number of 2-diregular digraphs with n nodes.
 * https://oeis.org/A005641
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over cycle types sigma of S_n: a(n) = Sum_{sigma}
 *            fix(sigma)/n!, where fix(sigma) counts 0-1 matrices with all row
 *            and column sums 2 that commute with sigma. Cells only interact
 *            between cycles of lengths L and L or L and 2L, so cycle lengths
 *            split into chains m, 2m, 4m, ... (m odd). Insi...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t u64;
static u64 MD;

static u64 md_pow(u64 b, u64 e) {
    u64 r = 1;
    b %= MD;
    while (e) {
        if (e & 1) r = r * b % MD;
        b = b * b % MD;
        e >>= 1;
    }
    return r;
}

static int isprime31(u64 x) {
    u64 i;
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (i = 3; i * i <= x; i += 2)
        if (x % i == 0) return 0;
    return 1;
}

static u64 get_prime(int k) {
    static u64 *ps = NULL;
    static int cnt = 0, cap = 0;
    static u64 cand = 2147483647ULL;
    while (cnt <= k) {
        while (!isprime31(cand)) cand -= 2;
        if (cnt == cap) {
            cap = cap ? 2 * cap : 64;
            ps = (u64 *)realloc(ps, (size_t)cap * sizeof(u64));
        }
        ps[cnt++] = cand;
        cand -= 2;
    }
    return ps[k];
}

static u64 res_mod(int N, u64 p) {
    int i, m;
    u64 *fact, *ifact, *tot, *G, *tmp;
    u64 inv2, inv4, inv64;
    MD = p;
    fact = (u64 *)malloc((size_t)(N + 2) * sizeof(u64));
    ifact = (u64 *)malloc((size_t)(N + 2) * sizeof(u64));
    fact[0] = 1;
    for (i = 1; i <= N + 1; i++) fact[i] = fact[i - 1] * (u64)i % p;
    ifact[N + 1] = md_pow(fact[N + 1], p - 2);
    for (i = N + 1; i > 0; i--) ifact[i - 1] = ifact[i] * (u64)i % p;
    inv2 = (p + 1) / 2;
    inv4 = inv2 * inv2 % p;
    inv64 = md_pow(inv2, 6);

    tot = (u64 *)calloc((size_t)(N + 1), sizeof(u64));
    G = (u64 *)calloc((size_t)(N + 1), sizeof(u64));
    tmp = (u64 *)calloc((size_t)(N + 1), sizeof(u64));
    tot[0] = 1;

    for (m = 1; m <= N; m += 2) {
        int curR = 0, d;
        long L;
        u64 *cur = (u64 *)calloc((size_t)(N + 1), sizeof(u64));
        cur[0] = 1;
        for (L = m; L <= N; L *= 2) {
            int J = (int)(N / L), U = curR, u, j, pp, c, r, a;
            u64 Lm = (u64)L % p, L2 = Lm * Lm % p, invL = md_pow(Lm, p - 2);
            u64 negh = (p - Lm * inv2 % p) % p;
            u64 A = L2 * L2 % p * inv64 % p, B = L2 * Lm % p * inv4 % p;
            u64 *H = (u64 *)malloc((size_t)(U + 1) * (J + 1) * sizeof(u64));
            u64 *cc = (u64 *)malloc((size_t)(J + 1) * sizeof(u64));
            u64 *ee = (u64 *)malloc((size_t)(J + 1) * sizeof(u64));
            u64 *S = (u64 *)malloc((size_t)(J + 1) * (U + 1) * sizeof(u64));
            u64 *Ap = (u64 *)malloc((size_t)(U + 1) * sizeof(u64));
            u64 *Bp = (u64 *)malloc((size_t)(U + 1) * sizeof(u64));
            u64 *base = (u64 *)malloc((size_t)(J + 1) * sizeof(u64));
            u64 *nxt = (u64 *)calloc((size_t)(N + 1) * (J + 1), sizeof(u64));

            ee[0] = 1;
            for (i = 1; i <= J; i++)
                ee[i] = ee[i - 1] * negh % p * (ifact[i] * fact[i - 1] % p) % p;
            for (u = 0; u <= U; u++) {
                cc[0] = 1;
                for (i = 1; i <= J; i++)
                    cc[i] = cc[i - 1] * ((u64)(2 * u + 2 * i - 1) % p) % p * inv2 % p
                            * (ifact[i] * fact[i - 1] % p) % p * L2 % p;
                for (j = 0; j <= J; j++) {
                    u64 s = 0;
                    for (i = 0; i <= j; i++) s = (s + cc[i] * ee[j - i]) % p;
                    H[(size_t)u * (J + 1) + j] = s;
                }
            }
            Ap[0] = 1; Bp[0] = 1;
            for (i = 1; i <= U; i++) {
                Ap[i] = Ap[i - 1] * A % p;
                Bp[i] = Bp[i - 1] * B % p;
            }
            for (pp = 0; pp <= J; pp++)
                for (u = 0; u <= U; u++) {
                    u64 s = 0;
                    for (a = 0; 2 * a <= u; a++) {
                        int b = u - 2 * a;
                        int jj = pp - u + a;
                        u64 t;
                        if (jj < 0) continue;
                        t = Ap[a] * Bp[b] % p * ifact[a] % p * ifact[a] % p * ifact[b] % p;
                        t = t * H[(size_t)u * (J + 1) + jj] % p;
                        s = (s + t) % p;
                    }
                    S[(size_t)pp * (U + 1) + u] = s;
                }
            base[0] = 1;
            for (c = 1; c <= J; c++) base[c] = base[c - 1] * (u64)c % p * invL % p;

            for (d = 0; d <= N; d++)
                for (u = 0; u <= U; u++) {
                    u64 v = cur[(size_t)d * (U + 1) + u];
                    if (!v) continue;
                    for (c = 0; (long)d + L * c <= N; c++) {
                        u64 bv = v * base[c] % p;
                        size_t row = (size_t)(d + L * c) * (J + 1);
                        for (r = 0; r <= c; r++) {
                            u64 s = S[(size_t)(c - r) * (U + 1) + u];
                            if (s) nxt[row + r] = (nxt[row + r] + bv * s) % p;
                        }
                    }
                }
            free(H); free(cc); free(ee); free(S); free(Ap); free(Bp); free(base);
            free(cur);
            cur = nxt;
            curR = J;
        }
        for (d = 0; d <= N; d++) G[d] = cur[(size_t)d * (curR + 1)];
        free(cur);
        for (d = 0; d <= N; d++) tmp[d] = 0;
        for (d = 0; d <= N; d++) {
            int k;
            if (!G[d]) continue;
            for (k = 0; k + d <= N; k++)
                if (tot[k]) tmp[k + d] = (tmp[k + d] + tot[k] * G[d]) % p;
        }
        memcpy(tot, tmp, (size_t)(N + 1) * sizeof(u64));
    }
    {
        u64 res = tot[N];
        free(fact); free(ifact); free(tot); free(G); free(tmp);
        return res;
    }
}

seq_t a(idx_t n) {
    int N = (int)n, lg = 0, k = 0, zeros = 0, i;
    long maxk;
    u64 *pr, *v;
    seq_t res = 0;
    while ((1ULL << lg) < (u64)n) lg++;
    maxk = (2L * N * lg) / 30 + 3;
    pr = (u64 *)malloc((size_t)maxk * sizeof(u64));
    v = (u64 *)malloc((size_t)maxk * sizeof(u64));
    while (k < maxk) {
        u64 q = get_prime(k), x, t = 0, M = 1, dg;
        x = res_mod(N, q);
        for (i = k - 1; i >= 0; i--) t = (t * (pr[i] % q) + v[i]) % q;
        for (i = 0; i < k; i++) M = M * (pr[i] % q) % q;
        MD = q;
        dg = (x + q - t) % q * md_pow(M, q - 2) % q;
        pr[k] = q;
        v[k] = dg;
        k++;
        if (dg == 0) zeros++; else zeros = 0;
        if (zeros >= 2) break;
    }
    for (i = k - 1; i >= 0; i--) res = res * (seq_t)pr[i] + (seq_t)v[i];
    free(pr);
    free(v);
    return res;
}
