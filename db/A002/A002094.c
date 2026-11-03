/*
 * A002094  Number of unlabeled connected loop-less graphs on n nodes
 *          containing exactly one cycle (of length at least 2) and with all
 *          nodes of degree <= 4.
 * https://oeis.org/A002094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   with A(x) = 1 + x*(A(x)^3 + 3*A(x)*A(x^2) + 2*A(x^3))/6
 *            (A000598), B(x) = (A(x)^2 + A(x^2))/2 and C(x) = x*B(x) (one
 *            cycle vertex with its trees), G.f. = Sum_{k>=2} Z(D_k)(C), which
 *            evaluates to a(n) = ( 2*Sum_{d|n} phi(d)*P(n/d) + n*T(n) -
 *            4*n*C(n) ) / (4*n), where P(x) = x*C'(x)/(1-C(x)) and T(...
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 91,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 399 (2026-11-03)
 * Helpers:   mem, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/totient_u64.h"
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

static uint64_t x2094_pw(uint64_t b, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    b %= p;
    while (e) {
        if (e & 1) r = r * b % p;
        b = b * b % p;
        e >>= 1;
    }
    return r;
}

/* a(n) mod p, p < 2^31 prime */
static uint64_t x2094_modrun(idx_t n, uint64_t p)
{
    idx_t L = n + 1, m, i;
    uint64_t *A = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *S2 = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *A3 = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *C = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *CC = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *T = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(L, sizeof(uint64_t));
    uint64_t inv6 = x2094_pw(6, p - 2, p), inv2 = x2094_pw(2, p - 2, p);
    uint64_t S = 0, num, d;
    A[0] = 1; S2[0] = 1; A3[0] = 1;
    for (m = 1; m <= n; m++) {
        idx_t k = m - 1;
        uint64_t s = A3[k], t = 0, q;
        for (i = 0; 2 * i <= k; i++) t = (t + A[i] * A[k - 2 * i]) % p;
        s = (s + 3 * t) % p;
        if (k % 3 == 0) s = (s + 2 * A[k / 3]) % p;
        A[m] = s * inv6 % p;
        s = 0;
        for (i = 0; i <= m; i++) s = (s + A[i] * A[m - i]) % p;
        S2[m] = s;
        s = 0;
        for (i = 0; i <= m; i++) s = (s + A[i] * S2[m - i]) % p;
        A3[m] = s;
        s = S2[k];
        if (k % 2 == 0) s = (s + A[k / 2]) % p;
        C[m] = s * inv2 % p;
        s = 0;
        for (i = 1; i < m; i++) s = (s + C[i] * C[m - i]) % p;
        CC[m] = s;
        q = (2 * C[m] + CC[m]) % p;
        if (m % 2 == 0) q = (q + C[m / 2]) % p;
        for (i = 1; 2 * i <= m; i++) q = (q + C[i] * T[m - 2 * i]) % p;
        T[m] = q;
        q = (uint64_t)(m % p) * C[m] % p;
        for (i = 1; i <= m; i++) q = (q + C[i] * P[m - i]) % p;
        P[m] = q;
    }
    for (d = 1; d <= n; d++)
        if (n % d == 0) S = (S + (totient_u64(d) % p) * P[n / d]) % p;
    num = (2 * S + (n % p) * T[n]) % p;
    num = (num + p - (4 * (n % p)) % p * C[n] % p) % p;
    num = num * x2094_pw((4 * (n % p)) % p, p - 2, p) % p;
    free(A); free(S2); free(A3); free(C); free(CC); free(T); free(P);
    return num;
}

static seq_t x2094_fixed(idx_t n)
{
    static const uint64_t pr[6] = {2147483647ULL, 2147483629ULL, 2147483587ULL,
                                   2147483579ULL, 2147483563ULL, 2147483549ULL};
    uint64_t r[6], c[6];
    seq_t res = 0, mult = 1;
    int i, j;
    for (i = 0; i < 6; i++) r[i] = x2094_modrun(n, pr[i]);
    for (i = 0; i < 6; i++) {
        uint64_t p = pr[i], v = 0, pm = 1;
        for (j = 0; j < i; j++) {
            v = (v + c[j] % p * pm) % p;
            pm = pm * (pr[j] % p) % p;
        }
        c[i] = (r[i] % p + p - v) % p * x2094_pw(pm, p - 2, p) % p;
    }
    for (i = 0; i < 6; i++) {
        res += (seq_t)c[i] * mult;
        mult *= (seq_t)pr[i];
    }
    return res;
}

static int x2094_width(void)
{
    seq_t x = 1;
    int w = 0;
    while (x != 0 && w < 400) { x *= 2; w++; }
    return w;
}

static seq_t *gA, *gS2, *gA3, *gC, *gCC, *gT, *gP;
static idx_t gN = 0, gCap = 0;

static void x2094_extend(idx_t upto)
{
    idx_t m, i;
    if (upto < gN) return;
    if (upto + 1 > gCap) {
        idx_t nc = gCap * 2;
        if (nc < upto + 1) nc = upto + 1;
        if (nc < 64) nc = 64;
        gA = grow_zero(gA, gCap, nc);
        gS2 = grow_zero(gS2, gCap, nc);
        gA3 = grow_zero(gA3, gCap, nc);
        gC = grow_zero(gC, gCap, nc);
        gCC = grow_zero(gCC, gCap, nc);
        gT = grow_zero(gT, gCap, nc);
        gP = grow_zero(gP, gCap, nc);
        gCap = nc;
    }
    for (m = gN; m <= upto; m++) {
        if (m == 0) {
            gA[0] = 1; gS2[0] = 1; gA3[0] = 1;
            gC[0] = 0; gCC[0] = 0; gT[0] = 0; gP[0] = 0;
            continue;
        }
        {
            idx_t k = m - 1;
            seq_t s = gA3[k], t = 0, q;
            for (i = 0; 2 * i <= k; i++) t += gA[i] * gA[k - 2 * i];
            s += 3 * t;
            if (k % 3 == 0) s += 2 * gA[k / 3];
            gA[m] = s / 6;
            s = 0;
            for (i = 0; 2 * i < m; i++) s += gA[i] * gA[m - i];
            s *= 2;
            if (m % 2 == 0) s += gA[m / 2] * gA[m / 2];
            gS2[m] = s;
            s = 0;
            for (i = 0; i <= m; i++) s += gA[i] * gS2[m - i];
            gA3[m] = s;
            s = gS2[k];
            if (k % 2 == 0) s += gA[k / 2];
            gC[m] = s / 2;
            s = 0;
            for (i = 1; 2 * i < m; i++) s += gC[i] * gC[m - i];
            s *= 2;
            if (m % 2 == 0) s += gC[m / 2] * gC[m / 2];
            gCC[m] = s;
            q = 2 * gC[m] + gCC[m];
            if (m % 2 == 0) q += gC[m / 2];
            for (i = 1; 2 * i <= m; i++) q += gC[i] * gT[m - 2 * i];
            gT[m] = q;
            q = (seq_t)m * gC[m];
            for (i = 1; i <= m; i++) q += gC[i] * gP[m - i];
            gP[m] = q;
        }
    }
    gN = upto + 1;
}

seq_t a(idx_t n)
{
    int w = x2094_width();
    if (w < 400) {
        if (n > (idx_t)(2 * w + 40)) return 0; /* a(n) cannot fit in seq_t */
        return x2094_fixed(n);
    }
    {
        seq_t S = 0, num;
        idx_t d;
        x2094_extend(n);
        for (d = 1; d <= n; d++)
            if (n % d == 0) S += (seq_t)totient_u64(d) * gP[n / d];
        num = 2 * S + (seq_t)n * gT[n];
        num -= (seq_t)n * 4 * gC[n];
        return num / ((seq_t)n * 4);
    }
}
