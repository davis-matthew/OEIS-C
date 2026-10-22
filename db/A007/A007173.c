/*
 * A007173  Number of simplicial 3-clusters with n cells.
 * https://oeis.org/A007173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = C(3n,n)/(3*(2n+1)*(2n+2)) + ([0==n mod 2]*C(3n/2,n) +
 *            [1==n mod 2]*C((3n-1)/2,(n-1)/2)) / (2n+2) + 2*([1==n mod
 *            3]*C(n,(n-1)/3) + [2==n mod 3]*C(n,(n-2)/3)) / (3n)
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 53,
 *            big any n
 * Helpers:   binom_exact, gcd_ull
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom_exact.h"
#include "../utils/gcd_ull.h"

static long long legendre(unsigned long long N, unsigned long long p) {
    long long e = 0;
    while (N) { N /= p; e += (long long)N; }
    return e;
}

seq_t a(idx_t n) {
    unsigned long long N = (unsigned long long)n;
    unsigned long long lim = 3 * N + 2;
    unsigned long long i, j, D = 1, r1;
    unsigned long long d2 = 2 * N + 2, d3 = 3 * N;
    unsigned long long M = 3 * N * (2 * N + 1) * (2 * N + 2);
    unsigned long long r2, r3, frac;
    long long *ex;
    char *comp;
    seq_t q1, q2, q3, B, C3;

    /* term 1 = (3n)! / (3 * n! * (2n+2)!) as prime exponents */
    comp = (char *)calloc(lim + 1, 1);
    ex = (long long *)calloc(lim + 1, sizeof(long long));
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
        ex[i] = legendre(3 * N, i) - legendre(N, i) - legendre(2 * N + 2, i);
        if (i == 3) ex[i] -= 1;
        if (ex[i] < 0) {
            long long t;
            for (t = 0; t < -ex[i]; t++) D *= i;
        }
    }
    /* floor and remainder of numerator / D, built incrementally */
    q1 = (seq_t)(1ULL / D);
    r1 = 1ULL % D;
    for (i = 2; i <= lim; i++) {
        long long t;
        if (comp[i] || ex[i] <= 0) continue;
        for (t = 0; t < ex[i]; t++) {
            unsigned long long rp = r1 * i;
            q1 = q1 * (seq_t)i + (seq_t)(rp / D);
            r1 = rp % D;
        }
    }
    free(comp);
    free(ex);

    /* term 2 */
    if (N % 2 == 0) B = binom_exact(3 * n / 2, n);
    else B = binom_exact((3 * n - 1) / 2, (n - 1) / 2);
    q2 = B / (seq_t)d2;
    r2 = (unsigned long long)(B % (seq_t)d2);

    /* term 3 */
    C3 = 0;
    if (N % 3 == 1) C3 = 2 * binom_exact(n, (n - 1) / 3);
    else if (N % 3 == 2) C3 = 2 * binom_exact(n, (n - 2) / 3);
    q3 = C3 / (seq_t)d3;
    r3 = (unsigned long long)(C3 % (seq_t)d3);

    frac = (r1 * (M / D) + r2 * (M / d2) + r3 * (M / d3)) / M;

    return q1 + q2 + q3 + (seq_t)frac;
}
