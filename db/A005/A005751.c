/*
 * A005751  Number of matched trees with 2n nodes.
 * https://oeis.org/A005751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let U(x) = x*Exp(M(x)) and M(x) = U(x)^2, where Exp is the Euler
 *            (multiset) transform. M counts rooted trees with a perfect
 *            matching and U counts rooted trees where only the root is
 *            unmatched. By Otter's dissimilarity theorem, a(n) = [x^(2n)] (
 *            (U(x)^2 + U(x^2))/2 - (M(x)^2 - M(x^2))/2 ). Exp(M)...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 58,
 *            big any n
 * Helpers:   grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

/* ---------- 256-bit path for fixed-width builds ---------- */
typedef struct { uint64_t w[4]; } W256;

static W256 w_from(uint64_t v) { W256 r; r.w[0] = v; r.w[1] = r.w[2] = r.w[3] = 0; return r; }

static W256 w_add(W256 a, W256 b)
{
    W256 r; unsigned __int128 c = 0; int i;
    for (i = 0; i < 4; i++) { c += (unsigned __int128)a.w[i] + b.w[i]; r.w[i] = (uint64_t)c; c >>= 64; }
    return r;
}

static W256 w_sub(W256 a, W256 b)
{
    W256 r; uint64_t borrow = 0; int i;
    for (i = 0; i < 4; i++) {
        uint64_t x = a.w[i], y = b.w[i];
        uint64_t d = x - y - borrow;
        borrow = (x < y || (x == y && borrow)) ? 1 : 0;
        r.w[i] = d;
    }
    return r;
}

static W256 w_mul(W256 a, W256 b)
{
    W256 r = w_from(0); int i, j;
    for (i = 0; i < 4; i++) {
        unsigned __int128 carry = 0;
        for (j = 0; i + j < 4; j++) {
            unsigned __int128 t = (unsigned __int128)a.w[i] * b.w[j] + r.w[i + j] + carry;
            r.w[i + j] = (uint64_t)t;
            carry = t >> 64;
        }
    }
    return r;
}

static W256 w_divu(W256 a, uint64_t d)
{
    W256 q; unsigned __int128 rem = 0; int i;
    for (i = 3; i >= 0; i--) {
        unsigned __int128 cur = (rem << 64) | a.w[i];
        q.w[i] = (uint64_t)(cur / d);
        rem = cur % d;
    }
    return q;
}

static seq_t a_fixed(idx_t n)
{
    idx_t N = 2 * n, i, j, k, d;
    W256 *U, *M, *E, *C, s, T;
    if (n > 70) return 0; /* far beyond any fixed width */
    U = (W256 *)malloc((N + 1) * sizeof(W256));
    M = (W256 *)malloc((N + 1) * sizeof(W256));
    E = (W256 *)malloc((N + 1) * sizeof(W256));
    C = (W256 *)malloc((N + 1) * sizeof(W256));
    for (i = 0; i <= N; i++) { U[i] = M[i] = E[i] = C[i] = w_from(0); }
    E[0] = w_from(1);
    for (i = 1; i <= N; i++) {
        U[i] = E[i - 1];
        if (i % 2 == 0) {
            s = w_from(0);
            for (j = 1; j < i - j; j += 2) s = w_add(s, w_mul(U[j], U[i - j]));
            s = w_add(s, s);
            if ((i / 2) % 2 == 1) s = w_add(s, w_mul(U[i / 2], U[i / 2]));
            M[i] = s;
            s = w_from(0);
            for (d = 1; d * d <= i; d++) {
                if (i % d) continue;
                if (d % 2 == 0) s = w_add(s, w_mul(w_from(d), M[d]));
                if (i / d != d && (i / d) % 2 == 0) s = w_add(s, w_mul(w_from(i / d), M[i / d]));
            }
            C[i] = s;
            s = w_from(0);
            for (k = 2; k <= i; k += 2) s = w_add(s, w_mul(C[k], E[i - k]));
            E[i] = w_divu(s, i);
        }
    }
    s = w_from(0);
    for (j = 2; j < N - j; j += 2) s = w_add(s, w_mul(M[j], M[N - j]));
    s = w_add(s, s);
    if (n % 2 == 0) s = w_add(s, w_mul(M[n], M[n]));
    T = w_add(w_add(M[N], U[n]), M[n]);
    T = w_divu(w_sub(T, s), 2);
    free(U); free(M); free(E); free(C);
    return u128_to_seq(((unsigned __int128)T.w[1] << 64) | T.w[0]);
}

/* ---------- arbitrary-precision path (memoized) ---------- */
static seq_t *bU, *bM, *bE, *bC;
static idx_t bcap = 0, bdone = 0;

static void b_extend(idx_t N)
{
    idx_t i, j, k, d;
    if (bcap < N + 1) {
        idx_t nc = (2 * bcap > N + 1) ? 2 * bcap : N + 1;
        bU = grow_zero(bU, bcap, nc);
        bM = grow_zero(bM, bcap, nc);
        bE = grow_zero(bE, bcap, nc);
        bC = grow_zero(bC, bcap, nc);
        if (bcap == 0) bE[0] = 1;
        bcap = nc;
    }
    for (i = bdone + 1; i <= N; i++) {
        bU[i] = bE[i - 1];
        if (i % 2 == 0) {
            seq_t s = 0;
            for (j = 1; j < i - j; j += 2) s += bU[j] * bU[i - j];
            s += s;
            if ((i / 2) % 2 == 1) s += bU[i / 2] * bU[i / 2];
            bM[i] = s;
            s = 0;
            for (d = 1; d * d <= i; d++) {
                if (i % d) continue;
                if (d % 2 == 0) s += (seq_t)d * bM[d];
                if (i / d != d && (i / d) % 2 == 0) s += (seq_t)(i / d) * bM[i / d];
            }
            bC[i] = s;
            s = 0;
            for (k = 2; k <= i; k += 2) s += bC[k] * bE[i - k];
            bE[i] = s / (seq_t)i;
        }
    }
    if (N > bdone) bdone = N;
}

static seq_t a_big(idx_t n)
{
    idx_t N = 2 * n, j;
    seq_t s = 0, T;
    b_extend(N);
    for (j = 2; j < N - j; j += 2) s += bM[j] * bM[N - j];
    s += s;
    if (n % 2 == 0) s += bM[n] * bM[n];
    T = bM[N] + bU[n] + bM[n];
    T -= s;
    return T / 2;
}

seq_t a(idx_t n)
{
    if (seq_is_big()) return a_big(n);
    return a_fixed(n);
}
