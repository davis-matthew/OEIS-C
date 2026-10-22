/*
 * A000349  One-half the number of permutations of length n with exactly 2
 *          rising or falling successions.
 * https://oeis.org/A000349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^2] S_n(t) / 2, where (A002464/A001100) S_n(t) =
 *            (n+1-t)*S_{n-1}(t) - (1-t)*(n-2+3t)*S_{n-2}(t) -
 *            (1-t)^2*(n-5+t)*S_{n-3}(t) + (1-t)^3*(n-3)*S_{n-4}(t). Since
 *            S_n(t) is linear in earlier terms and even for n >= 2, H_n =
 *            S_n/2 obeys the same recurrence. Only coefficients t^0..t^2 are
 *            trac...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   acc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/acc.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (n == 3) return 1;
    if (n == 4) return 5;
    if (n == 5) return 24;

    seq_t h[4][3];
    /* h[0]=H_{m-4}, h[1]=H_{m-3}, h[2]=H_{m-2}, h[3]=H_{m-1} */
    h[0][0] = 0; h[0][1] = 1;  h[0][2] = 0;
    h[1][0] = 0; h[1][1] = 2;  h[1][2] = 1;
    h[2][0] = 1; h[2][1] = 5;  h[2][2] = 5;
    h[3][0] = 7; h[3][1] = 20; h[3][2] = 24;

    idx_t m;
    for (m = 6; m <= n; m++) {
        long long mm = (long long)m;
        long long cf[4][3];
        /* coefficient polynomials, signs folded in */
        cf[3][0] = mm + 1;        cf[3][1] = -1;              cf[3][2] = 0;
        cf[2][0] = -(mm - 2);     cf[2][1] = -(5 - mm);       cf[2][2] = 3;
        cf[1][0] = -(mm - 5);     cf[1][1] = -(11 - 2 * mm);  cf[1][2] = -(mm - 7);
        cf[0][0] = mm - 3;        cf[0][1] = -3 * (mm - 3);   cf[0][2] = 3 * (mm - 3);

        seq_t nw[3];
        int k, i, r;
        for (k = 0; k < 3; k++) {
            seq_t pos = 0, neg = 0;
            for (r = 0; r < 4; r++)
                for (i = 0; i <= k; i++)
                    acc(&pos, &neg, cf[r][i], h[r][k - i]);
            nw[k] = pos - neg;
        }
        for (r = 0; r < 3; r++)
            for (k = 0; k < 3; k++)
                h[r][k] = h[r + 1][k];
        for (k = 0; k < 3; k++)
            h[3][k] = nw[k];
    }
    return h[3][2];
}
