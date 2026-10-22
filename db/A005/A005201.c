/*
 * A005201  Total number of fixed points in trees with n nodes.
 * https://oeis.org/A005201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   A(x) = T(x)*(1-F(x^2)) - F(x^2), where T(x) = Sum A000081(n) x^n
 *            satisfies A000081(1) = 1, A000081(n+1) = (1/n)*Sum_{k=1..n}
 *            (Sum_{d|k} d*A000081(d))*A000081(n-k+1), and F(x) = Sum
 *            A005200(n) x^n satisfies F(x) = T(x)*(1 + F(x) - F(x^2)). In
 *            fixed-width builds, it computes the terms modulo four p...
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 25, u64 n <= 47, u128 n <= 88,
 *            big any n
 * Helpers:   addm, grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/addm.h"

#define NP 4
static const uint64_t P[NP] = { 2305843009213693951ULL, 1000000007ULL,
                                1000000009ULL, 998244353ULL };

static uint64_t mulm(uint64_t x, uint64_t y, uint64_t p)
{
    return (uint64_t)((unsigned __int128)x * y % p);
}

static uint64_t powm(uint64_t x, uint64_t e, uint64_t p)
{
    uint64_t r = 1 % p;
    x %= p;
    while (e) {
        if (e & 1) r = mulm(r, x, p);
        x = mulm(x, x, p);
        e >>= 1;
    }
    return r;
}

/* ---------- modular path (fixed-width seq_t) ---------- */
static uint64_t *mT[NP], *mB[NP], *mF[NP], *mD[NP];
static idx_t mlen = 0;

static void mext(idx_t N)
{
    idx_t nl, m, k, d;
    int q;
    if (N < mlen) return;
    nl = N + 1;
    for (q = 0; q < NP; q++) {
        mT[q] = (uint64_t *)realloc(mT[q], nl * sizeof(uint64_t));
        mB[q] = (uint64_t *)realloc(mB[q], nl * sizeof(uint64_t));
        mF[q] = (uint64_t *)realloc(mF[q], nl * sizeof(uint64_t));
        mD[q] = (uint64_t *)realloc(mD[q], nl * sizeof(uint64_t));
    }
    for (m = (mlen ? mlen : 1); m <= N; m++) {
        for (q = 0; q < NP; q++) {
            uint64_t p = P[q];
            uint64_t *T = mT[q], *B = mB[q], *F = mF[q], *D = mD[q];
            uint64_t s;
            if (m == 1) {
                T[1] = 1;
            } else {
                s = 0;
                for (k = 1; k < m; k++)
                    s = addm(s, mulm(B[k], T[m - k], p), p);
                T[m] = mulm(s, powm((m - 1) % p, p - 2, p), p);
            }
            s = 0;
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    s = addm(s, mulm(d % p, T[d], p), p);
                    if (e != d) s = addm(s, mulm(e % p, T[e], p), p);
                }
            }
            B[m] = s;
            s = T[m];
            for (k = 1; k < m; k++)
                s = addm(s, mulm(T[k], D[m - k], p), p);
            F[m] = s;
            D[m] = (m % 2 == 0) ? addm(s, p - F[m / 2], p) : s;
        }
    }
    mlen = nl;
}

static seq_t a_mod(idx_t n)
{
    uint64_t r[NP], c[NP];
    unsigned __int128 R;
    int q, i, j;
    idx_t k;
    mext(n);
    for (q = 0; q < NP; q++) {
        uint64_t p = P[q];
        uint64_t *T = mT[q], *F = mF[q];
        uint64_t s = (n % 2 == 0) ? F[n / 2] : 0;
        for (k = 1; k < n; k++) {
            idx_t j2 = n - k;
            if (j2 % 2 == 0) s = addm(s, mulm(T[k], F[j2 / 2], p), p);
        }
        r[q] = addm(T[n], p - s, p);
    }
    c[0] = r[0];
    for (i = 1; i < NP; i++) {
        uint64_t p = P[i], v, pr = 1;
        v = c[i - 1] % p;
        for (j = i - 2; j >= 0; j--)
            v = addm(mulm(v, P[j] % p, p), c[j] % p, p);
        for (j = 0; j < i; j++) pr = mulm(pr, P[j] % p, p);
        c[i] = mulm(addm(r[i], p - v, p), powm(pr, p - 2, p), p);
    }
    R = c[NP - 1];
    for (j = NP - 2; j >= 0; j--)
        R = R * P[j] + c[j];
    return u128_to_seq(R);
}

/* ---------- exact path (arbitrary-precision seq_t) ---------- */
static seq_t *bT, *bB, *bF, *bD;
static idx_t blen = 0;

static void bext(idx_t N)
{
    idx_t nl, m, k, d;
    if (N < blen) return;
    nl = N + 1;
    bT = grow_zero(bT, blen, nl);
    bB = grow_zero(bB, blen, nl);
    bF = grow_zero(bF, blen, nl);
    bD = grow_zero(bD, blen, nl);
    for (m = (blen ? blen : 1); m <= N; m++) {
        seq_t s;
        if (m == 1) {
            bT[1] = 1;
        } else {
            s = 0;
            for (k = 1; k < m; k++)
                s += bB[k] * bT[m - k];
            bT[m] = s / (seq_t)(m - 1);
        }
        s = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                s += (seq_t)d * bT[d];
                if (e != d) s += (seq_t)e * bT[e];
            }
        }
        bB[m] = s;
        s = bT[m];
        for (k = 1; k < m; k++)
            s += bT[k] * bD[m - k];
        bF[m] = s;
        if (m % 2 == 0) bD[m] = s - bF[m / 2];
        else bD[m] = s;
    }
    blen = nl;
}

static seq_t a_big(idx_t n)
{
    seq_t s = 0;
    idx_t k;
    bext(n);
    if (n % 2 == 0) s = bF[n / 2];
    for (k = 1; k < n; k++) {
        idx_t j = n - k;
        if (j % 2 == 0) s += bT[k] * bF[j / 2];
    }
    return bT[n] - s;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (seq_is_big()) return a_big(n);
    return a_mod(n);
}
