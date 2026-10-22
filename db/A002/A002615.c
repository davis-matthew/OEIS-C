/*
 * A002615  Glaisher's function T_1(n).
 * https://oeis.org/A002615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = [q^n] ( theta_3(q)^16/32 + 18*theta_3(q)^12*F(q) -
 *            592*theta_3(q)^8*F(q)^2 + 4608*theta_3(q)^4*F(q)^3 + 2048*F(q)^4
 *            ), where F(q) = Sum_{k odd} sigma(k) q^k = theta_2(q)^4/16. Uses
 *            theta_3^8 = 1 + 16*Sum_n (Sum_{d|n} (-1)^(n+d) d^3) q^n,
 *            theta_3^4*F = Sum_n (Sum_{d|n, n/d odd} d^3) q^n, F^...
 * Range:     i8 n <= 1, i16 n <= 7, i32 n <= 32, i64 n <= 500, i128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static long long *a2615_A = NULL; /* A[k] = r8(k)/16 for k>=1 */
static long long *a2615_B = NULL; /* coefficients of theta_3^4 * F */
static long long *a2615_C = NULL; /* coefficients of F^2 */
static idx_t a2615_cap = 0;

static void a2615_build(idx_t N)
{
    idx_t d, m;
    free(a2615_A);
    free(a2615_B);
    free(a2615_C);
    a2615_A = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    a2615_B = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    a2615_C = (long long *)calloc((size_t)(N + 1), sizeof(long long));
    for (d = 1; d <= N; d++) {
        long long d3 = (long long)d * (long long)d * (long long)d;
        for (m = d; m <= N; m += d) {
            if ((m + d) & 1)
                a2615_A[m] -= d3;
            else
                a2615_A[m] += d3;
            if ((m / d) & 1)
                a2615_B[m] += d3;
        }
    }
    for (m = 2; m <= N; m += 2)
        a2615_C[m] = a2615_B[m / 2];
    a2615_cap = N;
}

seq_t a(idx_t n)
{
    seq_t s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0;
    idx_t k;
    if (n > a2615_cap) {
        idx_t N = n < 64 ? 64 : n;
        if (N < 2 * a2615_cap)
            N = 2 * a2615_cap;
        a2615_build(N);
    }
    for (k = 1; k < n; k++) {
        idx_t j = n - k;
        seq_t Ak = (seq_t)a2615_A[k];
        seq_t Bk = (seq_t)a2615_B[k];
        s1 += Ak * (seq_t)a2615_A[j];
        s2 += Ak * (seq_t)a2615_B[j];
        s3 += Bk * (seq_t)a2615_B[j];
        if (a2615_C[j] != 0)
            s4 += Bk * (seq_t)a2615_C[j];
        if (a2615_C[k] != 0 && a2615_C[j] != 0)
            s5 += (seq_t)a2615_C[k] * (seq_t)a2615_C[j];
    }
    s1 = (seq_t)a2615_A[n] + 8 * s1;   /* r16(n)/32 */
    s2 = (seq_t)a2615_B[n] + 16 * s2;  /* [q^n] theta_3^12 F */
    return s1 + 18 * s2 - 592 * s3 + 4608 * s4 + 2048 * s5;
}
