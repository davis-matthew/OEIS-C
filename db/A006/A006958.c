/*
 * A006958  Number of parallelogram polyominoes with n cells (also called
 *          staircase polyominoes, although that term is overused).
 * https://oeis.org/A006958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   1 + Sum_{n>=1} a(n)*q^n = N(q)/D(q), where N(q) = Sum_{n>=0}
 *            (-1)^n*q^((n^2+3*n)/2)/Product_{k=1..n} (1-q^k)^2 and D(q) =
 *            Sum_{n>=0} (-1)^n*q^((n^2+n)/2)/Product_{k=1..n} (1-q^k)^2. The
 *            positive and negative parts of N and D are kept apart, so every
 *            intermediate value is nonnegative
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 27, u64 n <= 54, u128 n <= 107,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *F_cache = NULL;
static idx_t F_len = 0;

static void build(idx_t M) {
    idx_t L = M + 1;
    seq_t *G = alloc_zero(L);
    seq_t *Dp = alloc_zero(L);
    seq_t *Dm = alloc_zero(L);
    seq_t *Np = alloc_zero(L);
    seq_t *Nm = alloc_zero(L);
    seq_t *F = alloc_zero(L);
    idx_t i, j, k;

    G[0] = 1;
    Dp[0] += 1;
    Np[0] += 1;
    for (k = 1;; k++) {
        idx_t T = k * (k + 1) / 2;
        idx_t E = T + k;
        int rep;
        if (T > M) break;
        /* G <- G / (1 - q^k)^2 */
        for (rep = 0; rep < 2; rep++)
            for (i = k; i <= M; i++)
                G[i] += G[i - k];
        {
            seq_t *dt = (k & 1) ? Dm : Dp;
            for (i = T; i <= M; i++)
                dt[i] += G[i - T];
        }
        if (E <= M) {
            seq_t *nt = (k & 1) ? Nm : Np;
            for (i = E; i <= M; i++)
                nt[i] += G[i - E];
        }
    }

    F[0] = 1;
    for (i = 1; i <= M; i++) {
        seq_t pos = Np[i];
        seq_t neg = Nm[i];
        for (j = 1; j <= i; j++) {
            pos += Dm[j] * F[i - j];
            neg += Dp[j] * F[i - j];
        }
        F[i] = pos - neg;
    }

    release(G, L);
    release(Dp, L);
    release(Dm, L);
    release(Np, L);
    release(Nm, L);

    if (F_cache != NULL) release(F_cache, F_len);
    F_cache = F;
    F_len = L;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n >= F_len) {
        idx_t M = n;
        if (F_len > 0 && 2 * F_len > M) M = 2 * F_len;
        if (M < 32) M = 32;
        build(M);
    }
    return F_cache[n];
}
