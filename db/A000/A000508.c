/*
 * A000508  Generalized class numbers c_(n,3).
 * https://oeis.org/A000508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..n-1} b_j*(61*n^6 - 75*n^4*j^2 + 15*n^2*j^4 -
 *            j^6), i.e. 6! [x^6] of Sum_j b_j*cos(j*x)/cos(n*x), where b_j is
 *            the normalized Gauss sum of the Kronecker character (4n|.) at j:
 *            with n = s^2*d (d squarefree), D the fundamental discriminant of
 *            Q(sqrt(d)), b_j = (W/(2s)) * Sum_{e | ra...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 17, u64 n <= 544,
 *            u128 n <= 10000, big any n
 * Helpers:   a508_chi, a508_jacobi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a508_chi.h"
#include "../utils/a508_jacobi.h"

seq_t a(idx_t n)
{
    long long A = (long long)n, x = A, s = 1, d = 1, D, p, j;
    long long pr[24];
    int np = 0, W, mask, i;
    long long *b;
    seq_t A2, A4, A6, P = 0, N = 0;

    pr[np++] = 2;
    for (p = 2; p * p <= x; p++) {
        int k = 0, t;
        if (x % p) continue;
        while (x % p == 0) { x /= p; k++; }
        for (t = 0; t < k / 2; t++) s *= p;
        if (k & 1) d *= p;
        if (k >= 2 && p != 2) pr[np++] = p;
    }
    if (x > 1) d *= x;
    if (d % 4 == 1) { D = d; W = 1; } else { D = 4 * d; W = 2; }

    b = (long long *)calloc((size_t)A, sizeof(long long));
    for (mask = 0; mask < (1 << np); mask++) {
        long long e = 1, L;
        int mu = 1, ce;
        for (i = 0; i < np; i++)
            if (mask >> i & 1) { e *= pr[i]; mu = -mu; }
        ce = a508_chi(D, e);
        if (!ce) continue;
        L = 4 * A / (e * D);
        for (j = 0; j < A; j += L) {
            int c = a508_chi(D, j / L);
            if (c) b[j] += (long long)(mu * ce * c) * (W * L);
        }
    }
    b[0] = (A == 1) ? b[0] / (2 * s) : b[0] / (4 * s);
    for (j = 1; j < A; j++) b[j] /= 2 * s;

    A2 = (seq_t)n * (seq_t)n;
    A4 = A2 * A2;
    A6 = A4 * A2;
    for (j = 0; j < A; j++) {
        long long bj = b[j];
        unsigned long long m;
        seq_t J, J2, J4, J6, M, u, v;
        if (!bj) continue;
        m = bj > 0 ? (unsigned long long)bj : (unsigned long long)(-bj);
        M = (seq_t)(idx_t)m;
        J = (seq_t)(idx_t)j;
        J2 = J * J;
        J4 = J2 * J2;
        J6 = J4 * J2;
        u = M * (61 * A6 + 15 * A2 * J4);
        v = M * (75 * A4 * J2 + J6);
        if (bj > 0) { P += u; N += v; }
        else { P += v; N += u; }
    }
    free(b);
    return P - N;
}
