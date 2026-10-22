/*
 * A001888  Hit polynomials.
 * https://oeis.org/A001888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} (-1)^(k-1)*k*r(n,k)*(n-k)!, where r(n,k) is
 *            the number of ways to place k non-attacking rooks on the board
 *            {(i,j): 0 <= j-i <= 2} (permutations with exactly one hit).
 *            Computed from the generating function: Q(z)*A(z) = P1(z) +
 *            P2(z)*Y(z), Y(z) = Sum_{m>=0} m!*rho^m, rho = z(1-z)/...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   a1888_acc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/a1888_acc.h"

static seq_t a1888_yp[8], a1888_ym[8], a1888_av[8];
static long long a1888_cur = -1;

static void a1888_step(long long n) {
    static const long long V[8] = {1, 0, -6, -3, 5, 0, -2, 1};
    static const long long L[8] = {0, 0, 1, 0, -3, 1, 2, -1};
    static const long long U[8] = {1, -1, -3, -3, 1, 3, -3, 1};
    static const long long P1[7] = {-3, -5, 3, 6, -3, -5, 3};
    static const long long P2[7] = {3, 3, -2, -4, 3, 3, -2};
    static const long long QA[8] = {0, 0, 4, -1, -5, 2, 2, -1};
    seq_t P = 0, M = 0;
    long long k, j;
    int s;

    if (n < 8) a1888_acc(&P, &M, V[n], (seq_t)1, (seq_t)0);
    for (k = 2; k <= 7; k++) {
        j = n - k + 1;
        if (j >= 0) a1888_acc(&P, &M, L[k] * j, a1888_yp[j & 7], a1888_ym[j & 7]);
    }
    for (k = 1; k <= 7; k++) {
        j = n - k;
        if (j >= 0) a1888_acc(&P, &M, -U[k], a1888_yp[j & 7], a1888_ym[j & 7]);
    }
    s = (int)(n & 7);
    if (P >= M) { a1888_yp[s] = P - M; a1888_ym[s] = 0; }
    else        { a1888_yp[s] = 0;     a1888_ym[s] = M - P; }

    P = 0; M = 0;
    if (n < 7) a1888_acc(&P, &M, P1[n], (seq_t)1, (seq_t)0);
    for (k = 0; k <= 6; k++) {
        j = n - k;
        if (j >= 0) a1888_acc(&P, &M, P2[k], a1888_yp[j & 7], a1888_ym[j & 7]);
    }
    for (k = 2; k <= 7; k++) {
        j = n - k;
        if (j >= 0) a1888_acc(&P, &M, QA[k], a1888_av[j & 7], (seq_t)0);
    }
    a1888_av[s] = P - M;
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    int i;
    if (N <= a1888_cur && N >= a1888_cur - 7 && N >= 0) return a1888_av[N & 7];
    if (N < a1888_cur) {
        for (i = 0; i < 8; i++) { a1888_yp[i] = 0; a1888_ym[i] = 0; a1888_av[i] = 0; }
        a1888_cur = -1;
    }
    while (a1888_cur < N) {
        a1888_cur++;
        a1888_step(a1888_cur);
    }
    return a1888_av[N & 7];
}
