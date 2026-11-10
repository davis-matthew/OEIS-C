/*
 * A089172  Values of n where numerators of series coefficients of 1/(1 +
 *          cosh(sqrt(x))) differ from A002425.
 * https://oeis.org/A089172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th index m >= 1 such that some odd prime p <= 2m
 *            divides the tangent number A000182(m+1) (odd part of numerator
 *            of [x^m] 1/(1+cosh(sqrt(x))) differs from A002425(m+1));
 *            computed by sieve over primes with Brent-Zimmermann recurrence
 *            mod p
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t *sq_list = 0;
static idx_t sq_cnt = 0;
static idx_t sq_N = 0;

static void sq_build(idx_t N) {
    idx_t lim = 2 * N + 2, p, i, j, k;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    unsigned char *mark = (unsigned char *)calloc((size_t)N + 2, 1);
    uint64_t *T = (uint64_t *)malloc(((size_t)N + 3) * sizeof(uint64_t));
    for (p = 2; p * p <= lim; p++)
        if (!comp[p]) for (i = p * p; i <= lim; i += p) comp[i] = 1;
    for (p = 3; p <= 2 * N; p++) {
        idx_t M = N + 1, start;
        if (comp[p]) continue;
        T[1] = 1 % p;
        for (k = 2; k <= M; k++) T[k] = ((uint64_t)(k - 1) % p) * T[k - 1] % p;
        for (k = 2; k <= M; k++)
            for (j = k; j <= M; j++)
                T[j] = (((uint64_t)(j - k) % p) * T[j - 1] + ((uint64_t)(j - k + 2) % p) * T[j]) % p;
        start = (p + 1) / 2;
        if (start < 1) start = 1;
        for (i = start; i <= N; i++)
            if (T[i + 1] == 0) mark[i] = 1;
    }
    free(sq_list);
    sq_list = (idx_t *)malloc(((size_t)N + 1) * sizeof(idx_t));
    sq_cnt = 0;
    for (i = 1; i <= N; i++)
        if (mark[i]) sq_list[sq_cnt++] = i;
    sq_N = N;
    free(comp); free(mark); free(T);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (sq_N == 0) sq_build(128);
    while (sq_cnt < n) sq_build(sq_N * 2);
    return (seq_t)sq_list[n - 1];
}
