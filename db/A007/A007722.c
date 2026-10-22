/*
 * A007722  Number of graphical partitions of biconnected graphs with n nodes.
 * https://oeis.org/A007722
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = G(n) - F(n). G(n) counts graphical partitions with n
 *            parts, all parts in 2..n-1. By the Wang-Kleitman theorem, such a
 *            sequence is potentially 2-connected exactly when
 *            sum_{j>=2}(d_j-2) >= d_1-2. The sequences that fail this test
 *            are all graphical, so F(n) = Sum_{e=2..n-3} Sum_{E<=e-2, E==e...
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 68,
 *            big any n
 * Helpers:   a007722_cap, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a007722_cap.h"

seq_t a(idx_t n)
{
    long long N = (long long)n, B = N - 2;
    long long Cmax = a007722_cap(N - 2), W = Cmax + 2, cur = Cmax;
    long long ap, bp, a1, D, Dn, b, e, k;
    seq_t *R = alloc_zero((idx_t)((B + 1) * W));
    seq_t *S = alloc_zero((idx_t)W);
    seq_t *P = alloc_zero((idx_t)W);
    seq_t *T = alloc_zero((idx_t)W);
    seq_t *sw;
    seq_t *p, *Q;
    seq_t ans = 0, F = 0;

    for (ap = N - 3; ap >= 0; ap--) {
        long long cap = a007722_cap(ap + 1), cn = a007722_cap(ap);
        if (cap < cur) {
            for (b = 0; b <= B; b++) {
                seq_t *Rb = R + b * W;
                for (D = cap + 2; D <= cur + 1; D++) {
                    Rb[cap + (D & 1)] += Rb[D];
                    Rb[D] = 0;
                }
            }
        }
        cur = cap;
        for (D = 0; D <= cap + 1; D++) { S[D] = 0; P[D] = 0; }
        /* chains starting (a1, n-1), (ap, n-2) */
        for (a1 = ap + 1; a1 <= N - 2; a1++) {
            D = (N - 2 - a1) + (N - 3 - ap);
            if (D > cn + 1) D = cn + (D & 1);
            P[D] += 1;
        }
        for (D = 0; D <= cn + 1; D += 2) ans += P[D];
        for (bp = B - 1; bp >= 0; bp--) {
            seq_t *Rb = R + (bp + 1) * W;
            long long st;
            for (D = 0; D <= cap + 1; D++) S[D] += Rb[D];
            for (D = 0; D <= cn + 1; D++) Rb[D] += P[D];
            for (D = 0; D <= cn + 1; D++) T[D] = 0;
            st = ap + 1 - bp;
            if (st < 0) st = 0;
            for (D = st; D <= cap + 1; D++) {
                Dn = D + bp - ap - 1;
                if (Dn > cn + 1) Dn = cn + (Dn & 1);
                T[Dn] += S[D];
            }
            for (D = 0; D <= cn + 1; D += 2) ans += T[D];
            sw = P; P = T; T = sw;
        }
        for (D = 0; D <= cn + 1; D++) R[D] += P[D];
    }

    /* F(n): sequences failing the biconnectivity condition */
    p = alloc_zero((idx_t)(N + 1));
    Q = alloc_zero((idx_t)(N + 1));
    p[0] = 1;
    for (k = 1; k <= N; k++)
        for (e = k; e <= N; e++) p[e] += p[e - k];
    for (e = 0; e <= N; e++) {
        Q[e] = p[e];
        if (e >= 2) Q[e] += Q[e - 2];
    }
    for (e = 2; e <= N - 3; e++) F += Q[e - 2];

    release(R, (idx_t)((B + 1) * W));
    release(S, (idx_t)W);
    release(P, (idx_t)W);
    release(T, (idx_t)W);
    release(p, (idx_t)(N + 1));
    release(Q, (idx_t)(N + 1));
    return ans - F;
}
