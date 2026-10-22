/*
 * A006712  Number of 3-edge-colored trivalent graphs with 2n labeled nodes.
 * https://oeis.org/A006712
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2n,2k)*(2k-1)!! *
 *            E[(t1*t2*t3)^(n-k) * (2-t1-t2-t3)^k], where E[t1^e1*t2^e2*t3^e3]
 *            = (2e1-1)!!*(2e2-1)!!*(2e3-1)!! (inclusion-exclusion over
 *            coinciding edges of three perfect matchings); expanded: a(n) =
 *            Sum_k C(2n,2k)*(2k-1)!! * Sum_r C(k,r)*2^(k-r)*(-1)^r *
 *            P_{n-k}(...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    idx_t W = n + 1;
    idx_t r, e, k, i, j;
    seq_t *B = alloc_zero(W * W);
    seq_t *R = alloc_zero(2 * n + 1);
    seq_t *f = alloc_zero(W);
    seq_t *pw = alloc_zero(W);
    seq_t *Q = alloc_zero(W);
    seq_t *P = alloc_zero(W);
    seq_t pos = 0, neg = 0, res;

    /* Pascal triangle rows 0..n */
    for (r = 0; r <= n; r++) {
        B[r * W] = 1;
        for (e = 1; e <= r; e++)
            B[r * W + e] = B[(r - 1) * W + e - 1] + B[(r - 1) * W + e];
    }
    /* row 2n of Pascal triangle */
    R[0] = 1;
    for (i = 1; i <= 2 * n; i++)
        for (j = i; j >= 1; j--)
            R[j] += R[j - 1];
    /* double factorials (2i-1)!! and powers of 2 */
    f[0] = 1;
    pw[0] = 1;
    for (i = 1; i <= n; i++) {
        f[i] = f[i - 1] * (seq_t)(2 * i - 1);
        pw[i] = pw[i - 1] * 2;
    }

    for (k = 0; k <= n; k++) {
        idx_t m = n - k;
        seq_t c;
        const seq_t *F = f + m;
        for (r = 0; r <= k; r++) {
            seq_t s = 0;
            for (e = 0; e <= r; e++)
                s += B[r * W + e] * F[e] * F[r - e];
            Q[r] = s;
        }
        for (r = 0; r <= k; r++) {
            seq_t s = 0;
            for (e = 0; e <= r; e++)
                s += B[r * W + e] * F[e] * Q[r - e];
            P[r] = s;
        }
        c = R[2 * k] * f[k];
        for (r = 0; r <= k; r++) {
            seq_t t = c * B[k * W + r] * pw[k - r] * P[r];
            if (r & 1)
                neg += t;
            else
                pos += t;
        }
    }
    res = pos - neg;

    release(B, W * W);
    release(R, 2 * n + 1);
    release(f, W);
    release(pw, W);
    release(Q, W);
    release(P, W);
    return res;
}
