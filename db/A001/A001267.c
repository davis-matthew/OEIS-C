/*
 * A001267  One-half the number of permutations of length n with exactly 3
 *          rising or falling successions.
 * https://oeis.org/A001267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^3] S[n](t) / 2, where S[n](t) (A002464) satisfies S[n]
 *            = (n+1-t)*S[n-1] - (1-t)*(n-2+3t)*S[n-2] -
 *            (1-t)^2*(n-5+t)*S[n-3] + (1-t)^3*(n-3)*S[n-4]. We run this
 *            recurrence on H[n] = S[n]/2, keeping only terms up to t^3, with
 *            H[2] = t, H[3] = 2t + t^2, H[4] = 1 + 5t + 5t^2 + t^3, H[5] = 7
 *            +...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   add_prod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/add_prod.h"

seq_t a(idx_t n) {
    seq_t h[4][4];
    seq_t pos[4], neg[4];
    idx_t m;
    int i, k;
    if (n < 4) return 0;
    for (k = 0; k < 4; k++)
        for (i = 0; i < 4; i++) h[k][i] = 0;
    h[0][1] = 1;
    h[1][1] = 2; h[1][2] = 1;
    h[2][0] = 1; h[2][1] = 5; h[2][2] = 5; h[2][3] = 1;
    h[3][0] = 7; h[3][1] = 20; h[3][2] = 24; h[3][3] = 8;
    if (n == 4) return h[2][3];
    if (n == 5) return h[3][3];
    for (m = 6; m <= n; m++) {
        long long nn = (long long)m;
        long long A[4], B[4], C[4], D[4];
        A[0] = nn + 1; A[1] = -1; A[2] = 0; A[3] = 0;
        B[0] = nn - 2; B[1] = 5 - nn; B[2] = -3; B[3] = 0;
        C[0] = nn - 5; C[1] = 11 - 2 * nn; C[2] = nn - 7; C[3] = 1;
        D[0] = nn - 3; D[1] = -3 * (nn - 3); D[2] = 3 * (nn - 3); D[3] = -(nn - 3);
        for (i = 0; i < 4; i++) { pos[i] = 0; neg[i] = 0; }
        add_prod(pos, neg, A, 1, h[3]);
        add_prod(pos, neg, B, -1, h[2]);
        add_prod(pos, neg, C, -1, h[1]);
        add_prod(pos, neg, D, 1, h[0]);
        for (k = 0; k < 3; k++)
            for (i = 0; i < 4; i++) h[k][i] = h[k + 1][i];
        for (i = 0; i < 4; i++) h[3][i] = pos[i] - neg[i];
    }
    return h[3][3];
}
