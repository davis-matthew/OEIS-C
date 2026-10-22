/*
 * A001885  Hit polynomials.
 * https://oeis.org/A001885
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^2] H_n(x), where H_n(x) = Sum_{j>=0}
 *            r_j*(n-j)!*(x-1)^j is the hit polynomial of the board {(i,j):
 *            |i-j|<=1} (n X n). The rook numbers satisfy Sum_n Sum_j r_j(n)
 *            y^j t^(n-j) z^n = (1-yz)/(1-(t+2y)z-tyz^2+y^3z^3). Writing that
 *            as (1-yz)/(P-tQ) and using E(R)=Sum m! R^m, which satisfies R...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   h1885_conv, h1885_pmul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/h1885_conv.h"
#include "../utils/h1885_pmul.h"

#define H1885_MAXN 20001

static long long h1885_A[10][3], h1885_B[10][3], h1885_C[10][3];
static int h1885_ready = 0;

static void h1885_init(void) {
    long long Q[10][12] = {{0}}, P[10][12] = {{0}}, Pd[10][12] = {{0}};
    long long L[10][12] = {{0}}, Nn[10][12] = {{0}}, Y[10][12] = {{0}};
    long long Q2[10][12], T1[10][12], Am[10][12], T2[10][12], T3[10][12];
    long long S[10][12], T4[10][12], QmP[10][12], T5[10][12], T6[10][12];
    long long Bm[10][12], T7[10][12], Cm[10][12];
    int i, j;
    Q[1][0] = 1; Q[2][1] = 1;
    P[0][0] = 1; P[1][1] = -2; P[3][3] = 1;
    Pd[0][1] = -2; Pd[2][3] = 3;
    L[0][0] = 1; L[1][1] = -1;
    Nn[0][0] = 1; Nn[1][1] = 2; Nn[2][2] = -2; Nn[3][3] = -2; Nn[4][4] = -1;
    Y[0][1] = 1;
    h1885_pmul(Q, Q, Q2);
    h1885_pmul(Q2, P, T1);
    h1885_pmul(T1, L, Am);
    h1885_pmul(Pd, L, T2);
    h1885_pmul(Y, P, T3);
    for (i = 0; i < 10; i++)
        for (j = 0; j < 12; j++) {
            S[i][j] = T2[i][j] + T3[i][j];
            QmP[i][j] = Q[i][j] - P[i][j];
        }
    h1885_pmul(Q2, S, T4);
    h1885_pmul(QmP, Nn, T5);
    h1885_pmul(T5, L, T6);
    for (i = 0; i < 10; i++)
        for (j = 0; j < 12; j++)
            Bm[i][j] = T4[i][j] + T6[i][j];
    h1885_pmul(Nn, L, T7);
    h1885_pmul(T7, L, Cm);
    h1885_conv(Am, h1885_A);
    h1885_conv(Bm, h1885_B);
    h1885_conv(Cm, h1885_C);
    h1885_ready = 1;
}

seq_t a(idx_t n) {
    static seq_t ans[H1885_MAXN];
    static unsigned char have[H1885_MAXN];
    static seq_t win[9][3];
    static idx_t done = 0;
    if (!h1885_ready) h1885_init();
    if (n < H1885_MAXN && have[n]) return ans[n];
    if (n < done) done = 0;
    while (done <= n) {
        idx_t r = done;
        idx_t lo = r >= 8 ? r - 8 : 0;
        seq_t cur[3];
        int k, b;
        for (k = 0; k < 3; k++) {
            seq_t pos = 0, neg = 0;
            idx_t m;
            for (m = lo; m < r; m++) {
                idx_t d = r - m;
                for (b = 0; b <= k; b++) {
                    int a2 = k - b;
                    long long K = h1885_B[d][a2];
                    if (d + 1 <= 9) K += (long long)m * h1885_A[d + 1][a2];
                    if (K > 0) pos += win[m % 9][b] * (seq_t)K;
                    else if (K < 0) neg += win[m % 9][b] * (seq_t)(-K);
                }
            }
            if (r < 10) {
                long long K = h1885_C[r][k];
                if (K > 0) pos += (seq_t)K;
                else if (K < 0) neg += (seq_t)(-K);
            }
            cur[k] = pos - neg;
        }
        for (k = 0; k < 3; k++) win[r % 9][k] = cur[k];
        if (r < H1885_MAXN) { ans[r] = cur[2]; have[r] = 1; }
        done++;
    }
    return win[n % 9][2];
}
