/*
 * A004675  Theta series of extremal even unimodular lattice in dimension 72.
 * https://oeis.org/A004675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. (in x = q^2) is the unique weight-36 modular form E4^9 +
 *            c1*E4^6*Delta + c2*E4^3*Delta^2 + c3*Delta^3 = 1 + O(x^4), E4 =
 *            1 + 240*Sum_{k>=1} sigma_3(k)*x^k, Delta = x*Prod_{k>=1}
 *            (1-x^k)^24, c1, c2, c3 chosen so the x^1..x^3 coefficients
 *            vanish. Computed modulo several NTT primes and recombin...
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 7, u128 n <= 26,
 *            big any n
 * Helpers:   is_prime, mod_ll, pw64, ser4_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_ll.h"
#include "../utils/pw64.h"
#include "../utils/ser4_mul.h"

static uint32_t MP, MNP, MR2;

static uint32_t m_redc(uint64_t T)
{
    uint32_t m = (uint32_t)T * MNP;
    uint64_t t = (T + (uint64_t)m * MP) >> 32;
    return t >= MP ? (uint32_t)(t - MP) : (uint32_t)t;
}
static uint32_t m_mul(uint32_t a, uint32_t b) { return m_redc((uint64_t)a * b); }
static uint32_t m_add(uint32_t a, uint32_t b) { uint32_t s = a + b; return s >= MP ? s - MP : s; }
static uint32_t m_sub(uint32_t a, uint32_t b) { return a >= b ? a - b : a + MP - b; }
static uint32_t m_to(uint32_t x) { return m_mul(x, MR2); }
static uint32_t m_pow(uint32_t b, uint64_t e)
{
    uint32_t r = m_to(1);
    while (e) { if (e & 1) r = m_mul(r, b); b = m_mul(b, b); e >>= 1; }
    return r;
}
static void m_setup(uint32_t p)
{
    uint32_t inv = p;
    int i;
    for (i = 0; i < 5; i++) inv *= 2u - p * inv;
    MP = p;
    MNP = (uint32_t)(0u - inv);
    {
        uint64_t r = ((uint64_t)1 << 32) % p;
        MR2 = (uint32_t)(r * r % p);
    }
}

static void ntt(uint32_t *a, uint32_t L, uint32_t root, uint32_t *tw)
{
    uint32_t i, j, len;
    for (i = 1, j = 0; i < L; i++) {
        uint32_t bit = L >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t t = a[i]; a[i] = a[j]; a[j] = t; }
    }
    for (len = 2; len <= L; len <<= 1) {
        uint32_t half = len >> 1;
        uint32_t wl = m_pow(root, L / len);
        tw[0] = m_to(1);
        for (j = 1; j < half; j++) tw[j] = m_mul(tw[j - 1], wl);
        for (i = 0; i < L; i += len)
            for (j = 0; j < half; j++) {
                uint32_t u = a[i + j], v = m_mul(a[i + j + half], tw[j]);
                a[i + j] = m_add(u, v);
                a[i + j + half] = m_sub(u, v);
            }
    }
}

#define MAXPR 64
static uint32_t *RES = NULL;
static uint32_t PR[MAXPR];
static uint64_t GINV[MAXPR][MAXPR];
static int NPR = 0;
static idx_t NC = 0;

static void build(idx_t N)
{
    uint32_t L = 1;
    int bl = 0, need, have = 0, k = 0, pi, i, j;
    idx_t t, m;
    uint64_t c;
    long long E[4] = {1, 240, 2160, 6720}, D[4] = {0, 1, -24, 252};
    long long E3[4], S0[4], S1[4], S2[4], D2[4], c1, c2, c3;
    uint64_t *s3;
    uint32_t *A, *Ev, *Dv, *tw;

    while ((idx_t)L < 9 * N) L <<= 1;
    for (t = N; t; t >>= 1) bl++;
    need = 35 * bl + 40;
    for (c = ((((uint64_t)1 << 31) - 2) / L); c >= 1 && have < need && k < MAXPR; c--) {
        uint64_t p = c * L + 1;
        int b = 0;
        if (!is_prime(p)) continue;
        PR[k++] = (uint32_t)p;
        while ((p >> (b + 1)) != 0) b++;
        have += b;
    }
    NPR = k;
    for (i = 0; i < NPR; i++)
        for (j = 0; j < i; j++)
            GINV[i][j] = pw64(PR[j] % PR[i], PR[i] - 2, PR[i]);

    ser4_mul(E3, E, E); ser4_mul(E3, E3, E);
    ser4_mul(S0, E3, E3); ser4_mul(S0, S0, E3);
    ser4_mul(S1, E3, E3); ser4_mul(S1, S1, D);
    ser4_mul(D2, D, D); ser4_mul(S2, E3, D2);
    c1 = -S0[1];
    c2 = -(S0[2] + c1 * S1[2]);
    c3 = -(S0[3] + c1 * S1[3] + c2 * S2[3]);

    s3 = (uint64_t *)calloc((size_t)N, sizeof(uint64_t));
    for (m = 1; m < N; m++) {
        uint64_t d3 = (uint64_t)m * m * m;
        idx_t q;
        for (q = m; q < N; q += m) s3[q] += d3;
    }

    free(RES);
    RES = (uint32_t *)malloc(sizeof(uint32_t) * (size_t)NPR * (size_t)N);
    A = (uint32_t *)malloc(sizeof(uint32_t) * L);
    Ev = (uint32_t *)malloc(sizeof(uint32_t) * L);
    Dv = (uint32_t *)malloc(sizeof(uint32_t) * L);
    tw = (uint32_t *)malloc(sizeof(uint32_t) * (L / 2 + 1));

    for (pi = 0; pi < NPR; pi++) {
        uint32_t p = PR[pi], root, rinv, linv, C1, C2, C3;
        uint64_t cc = (uint64_t)(p - 1) / L, fac[40], x;
        int nf = 0, f;
        uint64_t g;
        idx_t kk;

        m_setup(p);
        fac[nf++] = 2;
        x = cc;
        for (g = 2; g * g <= x; g++)
            if (x % g == 0) { fac[nf++] = g; while (x % g == 0) x /= g; }
        if (x > 1) fac[nf++] = x;
        for (g = 2;; g++) {
            bool ok = true;
            for (f = 0; f < nf; f++)
                if (pw64(g, (p - 1) / fac[f], p) == 1) { ok = false; break; }
            if (ok) break;
        }
        root = m_to((uint32_t)pw64(g, cc, p));
        rinv = m_pow(root, (uint64_t)L - 1);
        linv = m_to((uint32_t)pw64(L % p, p - 2, p));

        /* Delta = x * (Prod (1-x^k)^3)^8, Jacobi: Prod (1-x^k)^3 = Sum (-1)^k (2k+1) x^(k(k+1)/2) */
        memset(A, 0, sizeof(uint32_t) * L);
        for (kk = 0; kk * (kk + 1) / 2 < N; kk++) {
            uint32_t v = (uint32_t)((2 * kk + 1) % p);
            if ((kk & 1) && v) v = p - v;
            A[kk * (kk + 1) / 2] = m_to(v);
        }
        ntt(A, L, root, tw);
        for (i = 0; i < (int)L; i++) {
            uint32_t y = m_mul(A[i], A[i]);
            y = m_mul(y, y);
            A[i] = m_mul(y, y);
        }
        ntt(A, L, rinv, tw);
        memset(Dv, 0, sizeof(uint32_t) * L);
        for (m = 1; m < N; m++) Dv[m] = m_mul(A[m - 1], linv);

        memset(Ev, 0, sizeof(uint32_t) * L);
        Ev[0] = m_to(1);
        for (m = 1; m < N; m++)
            Ev[m] = m_to((uint32_t)((uint64_t)(240 % p) * (s3[m] % p) % p));

        ntt(Ev, L, root, tw);
        ntt(Dv, L, root, tw);
        C1 = m_to(mod_ll(c1, p));
        C2 = m_to(mod_ll(c2, p));
        C3 = m_to(mod_ll(c3, p));
        for (i = 0; i < (int)L; i++) {
            uint32_t e = Ev[i], d = Dv[i];
            uint32_t e3 = m_mul(m_mul(e, e), e), d2 = m_mul(d, d), r;
            r = m_add(e3, m_mul(C1, d));
            r = m_add(m_mul(r, e3), m_mul(C2, d2));
            r = m_add(m_mul(r, e3), m_mul(C3, m_mul(d2, d)));
            Ev[i] = r;
        }
        ntt(Ev, L, rinv, tw);
        for (m = 0; m < N; m++)
            RES[(size_t)pi * N + m] = m_redc((uint64_t)m_mul(Ev[m], linv));
    }

    free(A); free(Ev); free(Dv); free(tw); free(s3);
    NC = N;
}

seq_t a(idx_t n)
{
    uint64_t v[MAXPR];
    int i, j;
    seq_t x = 0;
    if (n >= NC) {
        idx_t N = 2 * NC;
        if (N < n + 1) N = n + 1;
        if (N < 64) N = 64;
        build(N);
    }
    for (i = 0; i < NPR; i++) {
        uint64_t p = PR[i], t = RES[(size_t)i * NC + n];
        for (j = 0; j < i; j++)
            t = (t + p - v[j] % p) % p * GINV[i][j] % p;
        v[i] = t;
    }
    for (i = NPR - 1; i >= 0; i--)
        x = x * (idx_t)PR[i] + (idx_t)v[i];
    return x;
}
