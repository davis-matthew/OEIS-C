/*
 * A001100  Triangle read by rows: T(n,k) = number of permutations of length n
 *          with exactly k rising or falling successions, for n >= 1, 0 <= k
 *          <= n-1.
 * https://oeis.org/A001100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, nonn
 * Formula:   S[n](t) = Sum_k T(n,k)*t^k with S[0] = 1, S[1] = 1, S[2] = 2*t,
 *            S[3] = 4*t+2*t^2, and for n >= 4: S[n] = (n+1-t)*S[n-1] -
 *            (1-t)*(n-2+3*t)*S[n-2] - (1-t)^2*(n-5+t)*S[n-3] +
 *            (1-t)^3*(n-3)*S[n-4]; a(n) = T(r,k) for the triangle read by
 *            rows (r >= 1, 0 <= k <= r-1)
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 91, u64 n <= 231,
 *            u128 n <= 595, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define A1100_MAXC 4200

static seq_t A1100_S[5][A1100_MAXC];
static seq_t A1100_P[A1100_MAXC];
static seq_t A1100_N[A1100_MAXC];

static void a1100_addc(idx_t k, long long c, seq_t v) {
    if (c > 0) A1100_P[k] += v * (idx_t)c;
    else if (c < 0) A1100_N[k] += v * (idx_t)(-c);
}

seq_t a(idx_t n) {
    idx_t row = 1;
    while (row * (row + 1) / 2 < n) row++;
    idx_t col = n - row * (row - 1) / 2 - 1;
    idx_t width = row + 2;
    if (width > A1100_MAXC) return 0;

    for (int s = 0; s < 5; s++)
        for (idx_t k = 0; k < width; k++) A1100_S[s][k] = 0;

    A1100_S[0][0] = 1;
    A1100_S[1][0] = 1;
    A1100_S[2][1] = 2;
    A1100_S[3][1] = 4;
    A1100_S[3][2] = 2;

    for (idx_t i = 4; i <= row; i++) {
        long long m = (long long)i;
        long long c = m - 5;
        seq_t *cur = A1100_S[i % 5];
        seq_t *s1 = A1100_S[(i - 1) % 5];
        seq_t *s2 = A1100_S[(i - 2) % 5];
        seq_t *s3 = A1100_S[(i - 3) % 5];
        seq_t *s4 = A1100_S[(i - 4) % 5];
        for (idx_t k = 0; k <= i + 1; k++) { A1100_P[k] = 0; A1100_N[k] = 0; }

        /* (m+1-t)*S[i-1] */
        for (idx_t k = 0; k + 2 <= i; k++) {
            seq_t v = s1[k];
            a1100_addc(k, m + 1, v);
            a1100_addc(k + 1, -1, v);
        }
        /* -(1-t)(m-2+3t)*S[i-2] = -(m-2) + (m-5)t + 3t^2 */
        for (idx_t k = 0; k + 3 <= i; k++) {
            seq_t v = s2[k];
            a1100_addc(k, -(m - 2), v);
            a1100_addc(k + 1, m - 5, v);
            a1100_addc(k + 2, 3, v);
        }
        /* -(1-t)^2(c+t)*S[i-3] = -c + (2c-1)t + (2-c)t^2 - t^3, c = m-5 */
        if (i >= 4) {
            for (idx_t k = 0; k + 4 <= i; k++) {
                seq_t v = s3[k];
                a1100_addc(k, -c, v);
                a1100_addc(k + 1, 2 * c - 1, v);
                a1100_addc(k + 2, 2 - c, v);
                a1100_addc(k + 3, -1, v);
            }
        }
        /* (m-3)(1-t)^3*S[i-4] */
        for (idx_t k = 0; k + 4 <= i || k == 0; k++) {
            seq_t v = s4[k];
            a1100_addc(k, m - 3, v);
            a1100_addc(k + 1, -3 * (m - 3), v);
            a1100_addc(k + 2, 3 * (m - 3), v);
            a1100_addc(k + 3, -(m - 3), v);
            if (k + 5 > i) break;
        }

        for (idx_t k = 0; k <= i + 1 && k < width; k++) cur[k] = A1100_P[k] - A1100_N[k];
    }

    return A1100_S[row % 5][col];
}
