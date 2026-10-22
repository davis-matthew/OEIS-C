/*
 * A000938  Number of collinear point-triples in an n X n grid.
 * https://oeis.org/A000938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 2*Sum_{k=2..n} Sum_{m=2..n} (n-k+1)*(n-m+1)*gcd(k-1,m-1)
 *            - n^2*(n^2-1)/6, evaluated via gcd(i,j) = Sum_{d|i,d|j} phi(d):
 *            Sum_{i,j=1..n-1} (n-i)(n-j)gcd(i,j) = Sum_{d=1..n-1} phi(d) *
 *            S_d^2, S_d = Sum_{t=1..q} (n - d*t) = q*n - d*q*(q+1)/2, q =
 *            floor((n-1)/d)
 * Range:     u8 n <= 5, u16 n <= 18, u32 n <= 235, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 3) return 0;
    idx_t N = n - 1;
    idx_t *phi = (idx_t *)malloc((size_t)(N + 1) * sizeof(idx_t));
    if (!phi) return 0;
    for (idx_t i = 0; i <= N; i++) phi[i] = i;
    for (idx_t p = 2; p <= N; p++) {
        if (phi[p] == p) {
            for (idx_t j = p; j <= N; j += p) phi[j] -= phi[j] / p;
        }
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= N; d++) {
        idx_t q = N / d;
        idx_t x = q, y = q + 1;
        if (x % 2 == 0) x /= 2; else y /= 2;
        idx_t S = q * n - d * (x * y);
        seq_t s = (seq_t)S;
        sum += (seq_t)phi[d] * s * s;
    }
    free(phi);
    sum = sum * 2;

    /* n^2(n^2-1)/6 = n * ((n-1)n(n+1)/6) */
    idx_t f1 = n - 1, f2 = n, f3 = n + 1;
    if (f1 % 2 == 0) f1 /= 2; else f2 /= 2;
    if (f1 % 3 == 0) f1 /= 3; else if (f2 % 3 == 0) f2 /= 3; else f3 /= 3;
    seq_t sub = (seq_t)f1 * (seq_t)f2 * (seq_t)f3 * (seq_t)n;
    return sum - sub;
}
