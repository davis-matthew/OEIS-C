/*
 * A071103  Number of perfect matchings in variant of n X n+2 Aztec rectangle
 *          graph.
 * https://oeis.org/A071103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |Pf(K)| where K is a Kasteleyn-oriented skew adjacency
 *            matrix of the planar graph in the definition (vertices (x,y)
 *            with 0<=x,y<=2n+1, n+1<=x+y<=3n+1, |y-x|<=n+2; diagonals exist
 *            exactly when x+y+n is odd). The Pfaffian is computed mod several
 *            32-bit primes and recombined by Garner CRT
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int A071103_orient(int i, int j, int dg, int pi, int qi, const int *px, const int *py)
{
    if ((i == pi && j == qi) || (i == qi && j == pi)) {
        return (i == pi) == (dg != 0);
    }
    if (py[i] == py[j]) {
        int right = (py[i] % 2 == 0);
        return ((px[j] > px[i]) == (right != 0));
    }
    return py[j] > py[i];
}

static uint32_t A071103_mulm(uint64_t a, uint64_t b, uint32_t p)
{
    return (uint32_t)(a * b % p);
}

static uint32_t A071103_powm(uint32_t b, uint32_t e, uint32_t p)
{
    uint32_t r = 1;
    while (e) {
        if (e & 1) r = A071103_mulm(r, b, p);
        b = A071103_mulm(b, b, p);
        e >>= 1;
    }
    return r;
}

static int A071103_isprime32(uint64_t x)
{
    uint64_t d;
    if (x < 2) return 0;
    if (x % 2 == 0) return x == 2;
    for (d = 3; d * d <= x; d += 2)
        if (x % d == 0) return 0;
    return 1;
}

static uint32_t A071103_pfaff(uint32_t *M, int N, uint32_t p,
                              const int *eu, const int *ev, int ne,
                              int *Sidx, uint32_t *Stau, int *Bidx, uint32_t *Bval)
{
    int k, e;
    uint32_t pf = 1;
    memset(M, 0, sizeof(uint32_t) * (size_t)N * (size_t)N);
    for (e = 0; e < ne; e++) {
        M[(size_t)eu[e] * N + ev[e]] = 1;
        M[(size_t)ev[e] * N + eu[e]] = p - 1;
    }
    for (k = 0; k + 1 < N; k += 2) {
        int j0 = -1, j, ns = 0, nb = 0, a, b;
        uint32_t inv;
        for (j = k + 1; j < N; j++) {
            if (M[(size_t)k * N + j] != 0) { j0 = j; break; }
        }
        if (j0 < 0) return 0;
        if (j0 != k + 1) {
            int t;
            for (t = 0; t < N; t++) {
                uint32_t tmp = M[(size_t)(k + 1) * N + t];
                M[(size_t)(k + 1) * N + t] = M[(size_t)j0 * N + t];
                M[(size_t)j0 * N + t] = tmp;
            }
            for (t = 0; t < N; t++) {
                uint32_t tmp = M[(size_t)t * N + k + 1];
                M[(size_t)t * N + k + 1] = M[(size_t)t * N + j0];
                M[(size_t)t * N + j0] = tmp;
            }
            pf = (pf == 0) ? 0 : p - pf;
        }
        pf = A071103_mulm(pf, M[(size_t)k * N + k + 1], p);
        inv = A071103_powm(M[(size_t)k * N + k + 1], p - 2, p);
        for (j = k + 2; j < N; j++) {
            uint32_t v = M[(size_t)k * N + j];
            if (v) {
                Sidx[ns] = j;
                Stau[ns] = A071103_mulm(v, inv, p);
                ns++;
            }
            v = M[(size_t)(k + 1) * N + j];
            if (v) {
                Bidx[nb] = j;
                Bval[nb] = v;
                nb++;
            }
        }
        for (a = 0; a < ns; a++) {
            int i = Sidx[a];
            uint32_t tau = Stau[a];
            for (b = 0; b < nb; b++) {
                int jj = Bidx[b];
                uint32_t t = A071103_mulm(tau, Bval[b], p);
                uint32_t *x = &M[(size_t)i * N + jj];
                uint32_t *y = &M[(size_t)jj * N + i];
                *x = (uint32_t)(((uint64_t)*x + p - t) % p);
                *y = (uint32_t)(((uint64_t)*y + t) % p);
            }
        }
    }
    return pf;
}

seq_t a(idx_t n)
{
    int m = (int)n;
    int W = 2 * m + 2;
    int *id = (int *)malloc(sizeof(int) * (size_t)W * (size_t)W);
    int N = 0, u, x, y, i;
    int *eu, *ev, ne = 0;
    int K;
    uint32_t *primes, *res, *dig;
    uint32_t *M;
    int *Sidx, *Bidx;
    uint32_t *Stau, *Bval;
    seq_t val, Mprod;
    uint64_t cand;
    int cnt;

    for (i = 0; i < W * W; i++) id[i] = -1;
    for (u = m + 1; u <= 3 * m + 1; u++) {
        for (x = 0; x <= 2 * m + 1; x++) {
            int d;
            y = u - x;
            if (y < 0 || y > 2 * m + 1) continue;
            d = y - x;
            if (d > m + 2 || d < -(m + 2)) continue;
            id[x * W + y] = N++;
        }
    }
    eu = (int *)malloc(sizeof(int) * (size_t)(8 * N + 16));
    ev = (int *)malloc(sizeof(int) * (size_t)(8 * N + 16));

    for (x = 0; x < W; x++) {
        for (y = 0; y < W; y++) {
            int a0 = id[x * W + y];
            if (a0 < 0) continue;
            if (x + 1 < W && id[(x + 1) * W + y] >= 0) {
                int b0 = id[(x + 1) * W + y];
                if (y % 2 == 0) { eu[ne] = a0; ev[ne] = b0; }
                else { eu[ne] = b0; ev[ne] = a0; }
                ne++;
            }
            if (y + 1 < W && id[x * W + y + 1] >= 0) {
                eu[ne] = a0; ev[ne] = id[x * W + y + 1];
                ne++;
            }
        }
    }
    for (x = 0; x + 1 < W; x++) {
        for (y = 0; y + 1 < W; y++) {
            int px[4], py[4], cid[4], pi, qi, odd, dg, chosen = 0, t;
            int tri[2][3];
            px[0] = x;     py[0] = y;
            px[1] = x + 1; py[1] = y;
            px[2] = x + 1; py[2] = y + 1;
            px[3] = x;     py[3] = y + 1;
            for (t = 0; t < 4; t++) cid[t] = id[px[t] * W + py[t]];
            odd = (x + y + m) & 1;
            if (odd) {
                pi = 0; qi = 2;
                tri[0][0] = 0; tri[0][1] = 1; tri[0][2] = 2;
                tri[1][0] = 0; tri[1][1] = 2; tri[1][2] = 3;
            } else {
                pi = 1; qi = 3;
                tri[0][0] = 0; tri[0][1] = 1; tri[0][2] = 3;
                tri[1][0] = 1; tri[1][1] = 2; tri[1][2] = 3;
            }
            dg = 0;
            for (t = 0; t < 2 && !chosen; t++) {
                int P = tri[t][0], Q = tri[t][1], R = tri[t][2], g;
                if (cid[P] < 0 || cid[Q] < 0 || cid[R] < 0) continue;
                for (g = 0; g < 2; g++) {
                    int c = A071103_orient(Q, P, g, pi, qi, px, py)
                          + A071103_orient(R, Q, g, pi, qi, px, py)
                          + A071103_orient(P, R, g, pi, qi, px, py);
                    if (c % 2 == 1) { dg = g; break; }
                }
                chosen = 1;
            }
            if (cid[pi] >= 0 && cid[qi] >= 0) {
                if (dg) { eu[ne] = cid[pi]; ev[ne] = cid[qi]; }
                else { eu[ne] = cid[qi]; ev[ne] = cid[pi]; }
                ne++;
            }
        }
    }

    K = N / 31 + 2;
    primes = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)K);
    res = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)K);
    dig = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)K);
    cand = 4294967291ULL;
    cnt = 0;
    while (cnt < K) {
        if (A071103_isprime32(cand)) primes[cnt++] = (uint32_t)cand;
        cand -= 2;
    }

    M = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)N * (size_t)N);
    Sidx = (int *)malloc(sizeof(int) * (size_t)N);
    Bidx = (int *)malloc(sizeof(int) * (size_t)N);
    Stau = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)N);
    Bval = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)N);
    for (i = 0; i < K; i++)
        res[i] = A071103_pfaff(M, N, primes[i], eu, ev, ne, Sidx, Stau, Bidx, Bval);

    /* Garner mixed-radix digits */
    for (i = 0; i < K; i++) {
        uint32_t p = primes[i];
        uint64_t s = 0, pref = 1;
        int j;
        uint32_t diff, invp;
        if (i > 0) {
            s = dig[i - 1] % p;
            for (j = i - 2; j >= 0; j--)
                s = ((uint64_t)s * primes[j] + dig[j]) % p;
        }
        for (j = 0; j < i; j++) pref = pref * (primes[j] % p) % p;
        diff = (uint32_t)(((uint64_t)res[i] + p - s) % p);
        if (i == 0) dig[i] = diff;
        else {
            invp = A071103_powm((uint32_t)pref, p - 2, p);
            dig[i] = A071103_mulm(diff, invp, p);
        }
    }
    {
        int neg = (dig[K - 1] != 0);
        int j;
        val = (seq_t)dig[K - 1];
        for (j = K - 2; j >= 0; j--)
            val = val * (seq_t)primes[j] + (seq_t)dig[j];
        if (neg) {
            Mprod = (seq_t)1;
            for (j = 0; j < K; j++) Mprod = Mprod * (seq_t)primes[j];
            val = Mprod - val;
        }
    }

    free(id); free(eu); free(ev); free(primes); free(res); free(dig);
    free(M); free(Sidx); free(Bidx); free(Stau); free(Bval);
    return val;
}
