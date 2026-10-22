/*
 * A007984  Number of essential graphs with n nodes (in 1-1 correspondence
 *          with Markov equivalence classes of acyclic digraphs).
 * https://oeis.org/A007984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{s=1..n} (-1)^(s+1) * binomial(n,s) * a(n-s) *
 *            Sum_{l=1..s} S2(s,l) * 2^((n-s)*l), with a(0) = 1, where S2 =
 *            Stirling numbers of the second kind. (This is
 *            inclusion-exclusion over the removable vertices: those with no
 *            children that are simplicial in their chain component. Each
 *            block of...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    idx_t N = n, i, j, t, s, l;
    idx_t tri = (N + 1) * (N + 2) / 2;
    seq_t *S = alloc_zero(tri);     /* Stirling 2nd kind, row i at i*(i+1)/2 */
    seq_t *B = alloc_zero(tri);     /* binomials, same layout */
    seq_t *p = alloc_zero(N + 1);   /* powers of 2 */
    seq_t *f = alloc_zero(N + 1);
    seq_t res;

    S[0] = 1;
    B[0] = 1;
    for (i = 1; i <= N; i++) {
        idx_t r = i * (i + 1) / 2, q = (i - 1) * i / 2;
        B[r] = 1;
        B[r + i] = 1;
        for (j = 1; j < i; j++)
            B[r + j] = B[q + j - 1] + B[q + j];
        for (j = 1; j <= i; j++) {
            seq_t v = S[q + j - 1];
            if (j <= i - 1)
                v += (seq_t)j * S[q + j];
            S[r + j] = v;
        }
    }
    p[0] = 1;
    for (i = 1; i <= N; i++)
        p[i] = p[i - 1] * 2;

    f[0] = 1;
    for (t = 1; t <= N; t++) {
        seq_t pos = 0, neg = 0;
        for (s = 1; s <= t; s++) {
            idx_t m = t - s;
            idx_t rs = s * (s + 1) / 2;
            seq_t x = p[m];
            seq_t T = 0, term;
            for (l = s; l >= 1; l--)
                T = (T + S[rs + l]) * x;
            term = B[t * (t + 1) / 2 + s] * f[m] * T;
            if (s & 1)
                pos += term;
            else
                neg += term;
        }
        f[t] = pos - neg;
    }
    res = f[N];
    release(S, tri);
    release(B, tri);
    release(p, N + 1);
    release(f, N + 1);
    return res;
}
