/*
 * A004010  Theta series of 12-dimensional Coxeter-Todd lattice K_12.
 * https://oeis.org/A004010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(0) = 1; for n >= 1, a(n) = (252/13) * (sigma_5(n) -
 *            27*sigma_5(n/3) - c(n)), where sigma_5(n/3) = 0 if 3 does not
 *            divide n, and c(n) is the coefficient of q^n in
 *            eta(q)^6*eta(q^3)^6 = q*Prod_{k>=1} (1-q^k)^6 (1-q^(3k))^6.
 *            (Theta_K12 = -E_6(q)/26 + 27*E_6(q^3)/26 - (252/13)*eta(q)^6
 *            eta(q^3)^6.)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 46, u64 n <= 3919,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static long long *P = NULL;  /* P[k] = c(k+1) */
static idx_t PL = 0;

static void build(idx_t len) {
    long long *AB = (long long *)calloc((size_t)len, sizeof(long long));
    long long *NP = (long long *)calloc((size_t)len, sizeof(long long));
    long long L = (long long)len;
    /* eta^3 = sum (-1)^m (2m+1) q^(m(m+1)/2) (without q^(1/8)) */
    for (long long m = 0; m * (m + 1) / 2 < L; m++) {
        long long t = m * (m + 1) / 2;
        long long sa = (m & 1) ? -(2 * m + 1) : (2 * m + 1);
        for (long long k = 0; t + 3 * (k * (k + 1) / 2) < L; k++) {
            long long u = 3 * (k * (k + 1) / 2);
            long long sb = (k & 1) ? -(2 * k + 1) : (2 * k + 1);
            AB[t + u] += sa * sb;
        }
    }
    for (long long i = 0; i < L; i++) {
        if (AB[i] == 0) continue;
        for (long long j = 0; i + j < L; j++)
            NP[i + j] += AB[i] * AB[j];
    }
    free(AB);
    free(P);
    P = NP;
    PL = len;
}

static seq_t pow5(idx_t d) {
    seq_t x = (seq_t)d;
    return x * x * x * x * x;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n > PL) {
        idx_t len = 2 * PL;
        if (len < n) len = n;
        if (len < 64) len = 64;
        build(len);
    }
    /* S = sigma_5(n) - 27*sigma_5(n/3) = sum over d|n of d^5, minus 27*d^5 for d with 3d|n */
    seq_t S = 0;
    for (idx_t d = 1; d * d <= n; d++) {
        if (n % d) continue;
        idx_t e = n / d;
        S += pow5(d);
        if (e != d) S += pow5(e);
    }
    if (n % 3 == 0) {
        idx_t m = n / 3;
        seq_t T = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d) continue;
            idx_t e = m / d;
            T += pow5(d);
            if (e != d) T += pow5(e);
        }
        S -= 27 * T;
    }
    long long c = P[n - 1];
    seq_t X = S;
    if (c >= 0) X -= (seq_t)(uint64_t)c;
    else X += (seq_t)(uint64_t)(-c);
    return X / 13 * 252;
}
