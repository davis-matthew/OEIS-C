/*
 * A002484  Number of ménage permutations.
 * https://oeis.org/A002484
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/n) * Sum_{d|n} phi(n/d) * G(d, n/d), where G(d, m) =
 *            Sum_{k=0..d} (-1)^k * (2d/(2d-k)) * binomial(2d-k, k) * (d-k)! *
 *            m^(d-k) is the number of menage permutations of Z_n commuting
 *            with rotation by d (Burnside's lemma over conjugation by
 *            rotations; G(n,1) = A000179(n))
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 35,
 *            big any n
 * Helpers:   a2484_big, a2484_big_G, a2484_isbig, a2484_tot
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/a2484_big.h"
#include "../utils/a2484_big_G.h"
#include "../utils/a2484_isbig.h"
#include "../utils/a2484_tot.h"

#define A2484_L 8

static void a2484_lmul(uint32_t *x, uint64_t m)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < A2484_L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void a2484_ladd(uint32_t *x, const uint32_t *y)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < A2484_L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void a2484_lsub(uint32_t *x, const uint32_t *y)
{
    uint64_t b = 0;
    int i;
    for (i = 0; i < A2484_L; i++) {
        uint64_t t = (uint64_t)x[i] - y[i] - b;
        x[i] = (uint32_t)t;
        b = (t >> 63) & 1;
    }
}

static void a2484_ladd128(uint32_t *x, unsigned __int128 v)
{
    uint32_t y[A2484_L];
    int i;
    for (i = 0; i < A2484_L; i++) {
        y[i] = (uint32_t)v;
        v >>= 32;
    }
    a2484_ladd(x, y);
}

static void a2484_ldiv(uint32_t *x, uint64_t n)
{
    uint64_t rem = 0;
    int i;
    for (i = A2484_L - 1; i >= 0; i--) {
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / n);
        rem = cur % n;
    }
}

/* fixed-width path: exact 256-bit arithmetic (exact whenever a(n) fits in 128 bits) */
static void a2484_fixed_G(idx_t d, idx_t m, uint32_t *G)
{
    uint32_t P[A2484_L], N[A2484_L];
    unsigned __int128 b1 = 1, b2 = 1;
    idx_t k;
    memset(P, 0, sizeof P);
    memset(N, 0, sizeof N);
    for (k = 0; k <= d; k++) {
        idx_t j = d - k;
        unsigned __int128 r;
        if (k > 0) {
            a2484_lmul(P, (uint64_t)((j + 1) * m));
            a2484_lmul(N, (uint64_t)((j + 1) * m));
        }
        r = b1;
        if (k > 0) r += b2;
        a2484_ladd128((k & 1) ? N : P, r);
        if (k < d) {
            b1 = b1 * (unsigned __int128)(2 * d - 2 * k);
            b1 = b1 * (unsigned __int128)(2 * d - 2 * k - 1);
            b1 = b1 / (unsigned __int128)((k + 1) * (2 * d - k));
        }
        if (k >= 1 && k < d) {
            idx_t jj = k - 1, N2 = 2 * d - 2;
            b2 = b2 * (unsigned __int128)(N2 - 2 * jj);
            b2 = b2 * (unsigned __int128)(N2 - 2 * jj - 1);
            b2 = b2 / (unsigned __int128)((jj + 1) * (N2 - jj));
        }
    }
    memcpy(G, P, sizeof P);
    a2484_lsub(G, N);
}

static seq_t a2484_fixed(idx_t n)
{
    uint32_t S[A2484_L], G[A2484_L];
    idx_t d;
    seq_t res = 0;
    int i;
    memset(S, 0, sizeof S);
    for (d = 1; d <= n; d++) {
        if (n % d != 0) continue;
        a2484_fixed_G(d, n / d, G);
        a2484_lmul(G, a2484_tot(n / d));
        a2484_ladd(S, G);
    }
    a2484_ldiv(S, n);
    for (i = A2484_L - 1; i >= 0; i--) {
        res = res * (seq_t)65536 * (seq_t)65536;
        res = res + (seq_t)S[i];
    }
    return res;
}

seq_t a(idx_t n)
{
    if (a2484_isbig()) return a2484_big(n);
    return a2484_fixed(n);
}
