/*
 * A005149  Sequence of coefficients arising in connection with a rapidly
 *          converging series for Pi.
 * https://oeis.org/A005149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, nice
 * Formula:   a(n) = REVERT(A014103), i.e. the inverse series of t(q) =
 *            q*Product_{k>=1} (1+q^k)^24. Uses q*dt/dq = t*W, W(t) =
 *            sqrt(1+64t)*2F1(1/4,1/4;1;-64t)^2, so that q/t = Sum_{n>=0}
 *            a(n+1) t^n = exp(Integral (1/W - 1) dt/t), i.e. with U = 1/W -
 *            1: e(0) = 1, n*e(n) = Sum_{k=1..n} u(k)*e(n-k), a(n) = e(n-1...
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 23,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/from_i128.h"
#include "../utils/seq_is_fixed_width.h"

#define FIXED_N 64

static unsigned __int128 *fy = NULL;

static void fixed_build(void)
{
    idx_t N = FIXED_N, j, m, i, k, r;
    unsigned __int128 *d = (unsigned __int128 *)calloc(N + 1, sizeof(unsigned __int128));
    unsigned __int128 *P = (unsigned __int128 *)calloc(N + 1, sizeof(unsigned __int128));
    unsigned __int128 *T = (unsigned __int128 *)calloc(N + 1, sizeof(unsigned __int128));
    unsigned __int128 *Y = (unsigned __int128 *)calloc((N + 1) * (N + 1), sizeof(unsigned __int128));
    fy = (unsigned __int128 *)calloc(N + 1, sizeof(unsigned __int128));
    d[0] = 1;
    for (k = 1; k <= N; k++)
        for (m = N; m >= k; m--)
            d[m] += d[m - k];
    P[0] = 1;
    for (r = 0; r < 24; r++) {
        for (m = 0; m <= N; m++) {
            unsigned __int128 s = 0;
            for (i = 0; i <= m; i++) s += P[i] * d[m - i];
            T[m] = s;
        }
        for (m = 0; m <= N; m++) P[m] = T[m];
    }
#define YY(jj, mm) Y[(jj) * (N + 1) + (mm)]
    fy[1] = 1;
    YY(1, 1) = 1;
    for (m = 2; m <= N; m++) {
        unsigned __int128 s = 0;
        for (j = 2; j <= m; j++) {
            unsigned __int128 v = 0;
            for (i = 1; i + j - 1 <= m; i++) v += fy[i] * YY(j - 1, m - i);
            YY(j, m) = v;
            s += P[j - 1] * v;
        }
        fy[m] = (unsigned __int128)0 - s;
        YY(1, m) = fy[m];
    }
#undef YY
    free(d); free(P); free(T); free(Y);
}

static seq_t *E = NULL;
static idx_t En = 0;

static void big_build(idx_t M)
{
    idx_t k, m;
    seq_t *b = alloc_zero(M), *fq = alloc_zero(M), *F2 = alloc_zero(M);
    seq_t *s = alloc_zero(M), *W = alloc_zero(M), *V = alloc_zero(M);
    seq_t *e = alloc_zero(M);
    b[0] = 1;
    s[0] = 1;
    for (k = 1; k < M; k++) {
        b[k] = b[k - 1] * 2 * ((seq_t)4 * (seq_t)k - 3) / (seq_t)k;
        s[k] = s[k - 1] * 32 * ((seq_t)3 - (seq_t)2 * (seq_t)k) / (seq_t)k;
    }
    for (k = 0; k < M; k++) {
        fq[k] = b[k] * b[k];
        if (k & 1) fq[k] = -fq[k];
    }
    for (m = 0; m < M; m++) {
        seq_t t = 0;
        for (k = 0; k <= m; k++) t += fq[k] * fq[m - k];
        F2[m] = t;
    }
    for (m = 0; m < M; m++) {
        seq_t t = 0;
        for (k = 0; k <= m; k++) t += s[k] * F2[m - k];
        W[m] = t;
    }
    V[0] = 1;
    for (m = 1; m < M; m++) {
        seq_t t = 0;
        for (k = 1; k <= m; k++) t += W[k] * V[m - k];
        V[m] = -t;
    }
    e[0] = 1;
    for (m = 1; m < M; m++) {
        seq_t t = 0;
        for (k = 1; k <= m; k++) t += V[k] * e[m - k];
        e[m] = t / (seq_t)m;
    }
    release(b, M); release(fq, M); release(F2, M);
    release(s, M); release(W, M); release(V, M);
    if (E) release(E, En);
    E = e;
    En = M;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (seq_is_fixed_width()) {
        if (n > FIXED_N) return 0;
        if (!fy) fixed_build();
        return from_i128((__int128)fy[n]);
    }
    if (n > En) {
        idx_t M = En * 2;
        if (M < n) M = n;
        if (M < 16) M = 16;
        big_build(M);
    }
    return E[n - 1];
}
