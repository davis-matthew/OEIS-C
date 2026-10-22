/*
 * A000649  Number of alkyls C_{n+15} H_{2n+10} (Phenan) with n carbon atoms.
 * https://oeis.org/A000649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: A(x) = (r(x)^10 + r(x^2)^5)/2, r(x) = A000598(x), where
 *            r(x) = 1 + x*(r(x)^3 + 3*r(x)*r(x^2) + 2*r(x^3))/6
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   sq_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sq_coef.h"

static seq_t *R = 0, *S2 = 0, *S3 = 0, *P4 = 0, *P8 = 0;
static idx_t cap = 0;

static void build(idx_t N) {
    idx_t m, i, j;
    if (R) { free(R); free(S2); free(S3); free(P4); free(P8); }
    R = (seq_t *)calloc(N, sizeof(seq_t));
    S2 = (seq_t *)calloc(N, sizeof(seq_t));
    S3 = (seq_t *)calloc(N, sizeof(seq_t));
    P4 = (seq_t *)calloc(N, sizeof(seq_t));
    P8 = (seq_t *)calloc(N, sizeof(seq_t));
    for (m = 0; m < N; m++) {
        if (m == 0) {
            R[0] = 1;
        } else {
            idx_t k = m - 1;
            seq_t t = S3[k];
            seq_t b = 0;
            for (j = 0; 2 * j <= k; j++)
                b += R[j] * R[k - 2 * j];
            t += 3 * b;
            if (k % 3 == 0)
                t += 2 * R[k / 3];
            R[m] = t / 6;
        }
        S2[m] = sq_coef(R, m);
        {
            seq_t s = 0;
            for (i = 0; i <= m; i++)
                s += S2[i] * R[m - i];
            S3[m] = s;
        }
        P4[m] = sq_coef(S2, m);
        P8[m] = sq_coef(P4, m);
    }
    cap = N;
}

seq_t a(idx_t n) {
    idx_t i;
    seq_t q = 0, rem = 0;
    if (n + 1 > cap) {
        idx_t nc = n + 1;
        if (nc < 2 * cap) nc = 2 * cap;
        if (nc < 64) nc = 64;
        build(nc);
    }
    /* r^10 = r^8 * r^2 */
    for (i = 0; i <= n; i++) {
        seq_t t = P8[i] * S2[n - i];
        q += t >> 1;
        rem += t & 1;
    }
    /* r(x^2)^5: coefficient of x^(n/2) in r^4 * r */
    if (n % 2 == 0) {
        idx_t h = n / 2;
        for (i = 0; i <= h; i++) {
            seq_t t = P4[i] * R[h - i];
            q += t >> 1;
            rem += t & 1;
        }
    }
    return q + rem / 2;
}
