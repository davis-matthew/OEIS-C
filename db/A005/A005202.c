/*
 * A005202  Total number of fixed points in planted trees with n nodes.
 * https://oeis.org/A005202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   let S(x) be the g.f. of planted series-reduced trees, counted
 *            without the plant root (S(x) = Sum_{n>=1} A001678(n+1) x^n), and
 *            M(x) = exp(Sum_{k>=1} S(x^k)/k) the Euler transform of S. Then
 *            S(x) = x*(M(x) - S(x)), and the g.f. F(x) of the total number of
 *            fixed points satisfies F(x) = S(x) + x*(M(...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 33, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"

#ifdef __cplusplus
typedef seq_t W;
static W *growW(W *p, idx_t oldlen, idx_t newlen)
{
    return grow_zero(p, oldlen, newlen);
}
#else
typedef unsigned __int128 W;
static W *growW(W *p, idx_t oldlen, idx_t newlen)
{
    W *q = (W *)realloc(p, (size_t)newlen * sizeof(W));
    idx_t i;
    for (i = oldlen; i < newlen; i++) q[i] = 0;
    return q;
}
#endif

static W *S = NULL, *M = NULL, *F = NULL, *D = NULL, *C = NULL;
static idx_t cap = 0;

static void extend(idx_t newcap)
{
    idx_t i, k, d;
    S = growW(S, cap, newcap);
    M = growW(M, cap, newcap);
    F = growW(F, cap, newcap);
    D = growW(D, cap, newcap);
    C = growW(C, cap, newcap);
    for (i = cap; i < newcap; i++) {
        if (i == 0) {
            S[0] = 0; M[0] = 1; F[0] = 0; D[0] = 0; C[0] = 0;
            continue;
        }
        S[i] = M[i - 1] - S[i - 1];
        {
            W c = 0;
            for (d = 1; d <= i; d++)
                if (i % d == 0) c += (W)d * S[d];
            C[i] = c;
        }
        {
            W sum = 0;
            for (k = 1; k <= i; k++) sum += C[k] * M[i - k];
            M[i] = sum / (W)i;
        }
        {
            W sum = S[i];
            for (k = 0; k < i; k++) sum += M[k] * D[i - 1 - k];
            sum -= F[i - 1];
            F[i] = sum;
        }
        if (i % 2 == 0) D[i] = F[i] - F[i / 2];
        else D[i] = F[i];
    }
    cap = newcap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > cap) {
        idx_t nc = cap * 2;
        if (nc < n) nc = n;
        if (nc < 16) nc = 16;
        extend(nc);
    }
    return (seq_t)F[n - 1];
}
