/*
 * A007239  Energy function for hexagonal lattice.
 * https://oeis.org/A007239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [v^n] U(v), U(v) = 3v + (1-v^2)*phi'(v), from the exact
 *            (Houtappel) solution of the triangular Ising model in v = tanh
 *            K: phi'(v) = (A'(v)/2)/A(v) - y'(v) * Sum_{m>=1} (W(m)/2)
 *            y^(m-1), A = (1+v^2)^3 + 8v^3, y = v(1-v^2)^2/A, W(m) =
 *            A002898(m) = number of closed walks of length m on the tr...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

/* w[m] = W(m)/2 for 0 <= m <= M */
static void half_walks(seq_t *w, idx_t M)
{
    idx_t m, k;
    if (seq_is_big()) {
        seq_t *b = alloc_zero(M + 2);
        seq_t *p3 = alloc_zero(M + 2);
        b[0] = 1; b[1] = 3;
        for (k = 1; k + 1 <= M; k++)
            b[k + 1] = ((seq_t)(10 * k * k + 10 * k + 3) * b[k]
                        - (seq_t)(9 * k * k) * b[k - 1]) / (seq_t)((k + 1) * (k + 1));
        p3[0] = 1;
        for (k = 1; k <= M; k++) p3[k] = p3[k - 1] * 3;
        w[0] = 0;
        for (m = 1; m <= M; m++) {
            seq_t pos = 0, neg = 0, c = 1;
            for (k = 0; k <= m; k++) {
                seq_t t = c * p3[m - k] * b[k];
                if ((m - k) & 1) neg += t; else pos += t;
                c = c * (seq_t)(m - k) / (seq_t)(k + 1);
            }
            w[m] = (pos - neg) / 2;
        }
        release(b, M + 2);
        release(p3, M + 2);
    } else {
        /* W(m)/2 = 3 * (walks of length m-1 from origin to a neighbour) */
        long long R = (long long)(M / 2) + 2, S = 2 * R + 1, x, y, d;
        static const int dx[6] = {1, -1, 0, 0, 1, -1};
        static const int dy[6] = {0, 0, 1, -1, 1, -1};
        seq_t *cur = alloc_zero((idx_t)(S * S));
        seq_t *nxt = alloc_zero((idx_t)(S * S));
        seq_t *tmp;
        idx_t t;
        cur[R * S + R] = 1;
        w[0] = 0;
        for (t = 0; t + 1 <= M; t++) {
            w[t + 1] = cur[(R + 1) * S + R] * 3;
            if (t + 2 > M) break;
            for (x = 0; x < S * S; x++) nxt[x] = 0;
            for (x = 0; x < S; x++)
                for (y = 0; y < S; y++) {
                    seq_t v = cur[x * S + y];
                    if (v == 0) continue;
                    for (d = 0; d < 6; d++) {
                        long long X = x + dx[d], Y = y + dy[d];
                        if (X < 0 || Y < 0 || X >= S || Y >= S) continue;
                        nxt[X * S + Y] += v;
                    }
                }
            tmp = cur; cur = nxt; nxt = tmp;
        }
        release(cur, (idx_t)(S * S));
        release(nxt, (idx_t)(S * S));
    }
}

static seq_t *cache = NULL;
static idx_t cacheN = 0;

static void compute(idx_t N)
{
    static const int Ac[7] = {1, 0, 3, 8, 3, 0, 1};
    static const int Ah[6] = {0, 3, 12, 6, 0, 3};
    idx_t L = N + 1, k, i, j;
    seq_t *w = alloc_zero(N + 2);
    seq_t *invA = alloc_zero(L), *T1 = alloc_zero(L), *B = alloc_zero(L);
    seq_t *vB = alloc_zero(L), *yp = alloc_zero(L), *Sm = alloc_zero(L);
    seq_t *P = alloc_zero(L), *Q = alloc_zero(L), *phi = alloc_zero(L), *tmp;
    seq_t *res;

    half_walks(w, N + 1);

    invA[0] = 1;
    for (k = 1; k <= N; k++) {
        seq_t s = 0;
        for (j = 1; j <= 6 && j <= k; j++)
            if (Ac[j]) s += (seq_t)Ac[j] * invA[k - j];
        invA[k] = (seq_t)0 - s;
    }
    for (k = 0; k <= N; k++) {
        seq_t s = 0;
        for (j = 1; j <= 5 && j <= k; j++)
            if (Ah[j]) s += (seq_t)Ah[j] * invA[k - j];
        T1[k] = s;
        B[k] = invA[k];
        if (k >= 2) B[k] -= invA[k - 2] * 2;
        if (k >= 4) B[k] += invA[k - 4];
    }
    for (k = 0; k <= N; k++) {
        vB[k] = (k >= 1) ? B[k - 1] : (seq_t)0;
        yp[k] = (seq_t)(k + 1) * B[k];
    }

    /* Sm = Sum_{j>=0} w[j+1] * (vB)^j */
    P[0] = 1;
    for (k = 0; k <= N; k++) Sm[k] = 0;
    Sm[0] = w[1];
    for (j = 1; j <= N; j++) {
        for (k = 0; k <= N; k++) {
            seq_t s = 0;
            if (k >= j)
                for (i = j - 1; i + 1 <= k; i++)
                    s += P[i] * vB[k - i];
            Q[k] = s;
        }
        tmp = P; P = Q; Q = tmp;
        for (k = j; k <= N; k++) Sm[k] += w[j + 1] * P[k];
    }

    for (k = 0; k <= N; k++) {
        seq_t s = 0;
        for (i = 0; i <= k; i++) s += yp[i] * Sm[k - i];
        phi[k] = T1[k] - s;
    }

    res = alloc_zero(L);
    for (k = 1; k <= N; k++) {
        seq_t v = phi[k];
        if (k >= 2) v -= phi[k - 2];
        if (k == 1) v += 3;
        res[k] = v;
    }

    if (cache) release(cache, cacheN + 1);
    cache = res;
    cacheN = N;

    release(w, N + 2);
    release(invA, L); release(T1, L); release(B, L);
    release(vB, L); release(yp, L); release(Sm, L);
    release(P, L); release(Q, L); release(phi, L);
}

seq_t a(idx_t n)
{
    if (n > cacheN || cache == NULL) {
        idx_t N = n < 32 ? 32 : n;
        compute(N);
    }
    return cache[n];
}
