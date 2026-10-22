/*
 * A000130  One-half the number of permutations of length n with exactly 1
 *          rising or falling succession.
 * https://oeis.org/A000130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c1(n)/2, where c0(n), c1(n) are the coefficients of t^0,
 *            t^1 in S[n](t) of A002464: S[n](t) = (n+1-t)S[n-1] -
 *            (1-t)(n-2+3t)S[n-2] - (1-t)^2(n-5+t)S[n-3] + (1-t)^3(n-3)S[n-4],
 *            giving c0(n) = (n+1)c0(n-1) - (n-2)c0(n-2) - (n-5)c0(n-3) +
 *            (n-3)c0(n-4), c1(n) = (n+1)c1(n-1) - c0(n-1) - (n-2)c1(...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const int b0[6] = {1, 1, 0, 0, 2, 14};
    static const int b1[6] = {0, 0, 2, 4, 10, 40};
    seq_t p0[4], p1[4];   /* p[0]=c(k-1), p[1]=c(k-2), p[2]=c(k-3), p[3]=c(k-4) */
    idx_t k;
    int i;

    if (n < 6)
        return (seq_t)(b1[n] / 2);

    for (i = 0; i < 4; i++) {
        p0[i] = (seq_t)b0[5 - i];
        p1[i] = (seq_t)b1[5 - i];
    }

    for (k = 6; k <= n; k++) {
        seq_t K = (seq_t)k;
        seq_t pos0, neg0, pos1, neg1, n0, n1;

        pos0 = (K + 1) * p0[0] + (K - 3) * p0[3];
        neg0 = (K - 2) * p0[1] + (K - 5) * p0[2];
        n0 = pos0 - neg0;

        pos1 = (K + 1) * p1[0] + (K - 5) * p0[1] + (2 * K - 11) * p0[2]
             + (K - 3) * p1[3];
        neg1 = p0[0] + (K - 2) * p1[1] + (K - 5) * p1[2] + 3 * (K - 3) * p0[3];
        n1 = pos1 - neg1;

        p0[3] = p0[2]; p0[2] = p0[1]; p0[1] = p0[0]; p0[0] = n0;
        p1[3] = p1[2]; p1[2] = p1[1]; p1[1] = p1[0]; p1[0] = n1;
    }
    return p1[0] / 2;
}
