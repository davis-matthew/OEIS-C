/*
 * A002273  Theta series of 28-dimensional Quebbemann lattice.
 * https://oeis.org/A002273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   theta series is a weight-14 modular form for Gamma_0(2): Theta =
 *            F^7 - 168*F^5*G + 107520*F^3*G^2 + 10321920*F*G^3, where F =
 *            2*E2(2z) - E2(z) = 1 + 24*Sum_{n>=1} sigma_1^odd(n) q^n (sum of
 *            odd divisors), G = eta(2z)^16/eta(z)^8 = Sum_{n>=1} (Sum_{d|n,
 *            n/d odd} d^3) q^n. With H = F^2 = E4(2z) + 4...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   a2273_u2s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2273_u2s.h"

static uint64_t *A2273_F = 0, *A2273_G = 0, *A2273_H = 0;
static unsigned __int128 *A2273_FH = 0, *A2273_FG = 0, *A2273_H2 = 0, *A2273_G2 = 0;
static idx_t A2273_N = 0; /* coefficients 0..A2273_N-1 are valid */

static void a2273_build(idx_t M)
{
    idx_t len = M + 1, d, m, k, i;
    uint64_t *s3;
    free(A2273_F); free(A2273_G); free(A2273_H);
    free(A2273_FH); free(A2273_FG); free(A2273_H2); free(A2273_G2);
    A2273_F = (uint64_t *)calloc(len, sizeof(uint64_t));
    A2273_G = (uint64_t *)calloc(len, sizeof(uint64_t));
    A2273_H = (uint64_t *)calloc(len, sizeof(uint64_t));
    s3 = (uint64_t *)calloc(len, sizeof(uint64_t));
    A2273_FH = (unsigned __int128 *)calloc(len, sizeof(unsigned __int128));
    A2273_FG = (unsigned __int128 *)calloc(len, sizeof(unsigned __int128));
    A2273_H2 = (unsigned __int128 *)calloc(len, sizeof(unsigned __int128));
    A2273_G2 = (unsigned __int128 *)calloc(len, sizeof(unsigned __int128));

    for (d = 1; d <= M; d++) {
        uint64_t d3 = (uint64_t)d * d * d;
        for (m = d; m <= M; m += d) {
            s3[m] += d3;
            if (d & 1) A2273_F[m] += 24 * (uint64_t)d;
            if ((m / d) & 1) A2273_G[m] += d3;
        }
    }
    A2273_F[0] = 1;
    A2273_G[0] = 0;
    A2273_H[0] = 1;
    for (m = 1; m <= M; m++)
        A2273_H[m] = 48 * A2273_G[m] + ((m & 1) ? 0 : 240 * s3[m / 2]);
    free(s3);

    for (k = 0; k <= M; k++) {
        unsigned __int128 fh = 0, fg = 0, h2 = 0, g2 = 0;
        for (i = 0; i <= k; i++) {
            fh += (unsigned __int128)A2273_F[i] * A2273_H[k - i];
            fg += (unsigned __int128)A2273_F[i] * A2273_G[k - i];
        }
        for (i = 0; 2 * i < k; i++) {
            h2 += (unsigned __int128)A2273_H[i] * A2273_H[k - i];
            g2 += (unsigned __int128)A2273_G[i] * A2273_G[k - i];
        }
        h2 *= 2;
        g2 *= 2;
        if ((k & 1) == 0) {
            h2 += (unsigned __int128)A2273_H[k / 2] * A2273_H[k / 2];
            g2 += (unsigned __int128)A2273_G[k / 2] * A2273_G[k / 2];
        }
        A2273_FH[k] = fh;
        A2273_FG[k] = fg;
        A2273_H2[k] = h2;
        A2273_G2[k] = g2;
    }
    A2273_N = len;
}

seq_t a(idx_t n)
{
    idx_t i;
    seq_t pos = 0, neg = 0;
    if (n >= A2273_N) {
        idx_t M = n;
        idx_t grow = A2273_N + A2273_N / 2;
        if (grow > M) M = grow;
        if (M < 64) M = 64;
        a2273_build(M);
    }
    for (i = 0; i <= n; i++) {
        idx_t j = n - i;
        seq_t fh = a2273_u2s(A2273_FH[i]);
        seq_t fg = a2273_u2s(A2273_FG[i]);
        seq_t h2 = a2273_u2s(A2273_H2[j]);
        seq_t g2 = a2273_u2s(A2273_G2[j]);
        seq_t t, u;
        t = fh * h2;
        pos += t;
        t = g2 * (seq_t)107520;
        t = fh * t;
        pos += t;
        u = g2 * (seq_t)10321920;
        u = fg * u;
        pos += u;
        t = h2 * (seq_t)168;
        t = fg * t;
        neg += t;
    }
    return pos - neg;
}
