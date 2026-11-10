/*
 * A203420  a(n) = A203418(n)/A000178(n).
 * https://oeis.org/A203420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A203418(n)/A000178(n-1) = prod_{1<=i<j<=n} (c(j)-c(i)) /
 *            prod_{m=1}^{n-1} m^(n-m), where c(k) is the k-th composite
 *            (A002808). The quotient is computed exactly through prime
 *            exponents (the per-step quotients are not integers)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 138 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static void a203420_build_spf(uint64_t lim, uint64_t *spf) {
    for (uint64_t i = 0; i <= lim; i++) spf[i] = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (spf[i] == 0) {
            for (uint64_t j = i; j <= lim; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
}

static void a203420_add_factors(uint64_t m, const uint64_t *spf,
                                long long *ex, long long mult) {
    while (m > 1) {
        uint64_t p = spf[m];
        while (m % p == 0) { m /= p; ex[p] += mult; }
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* find a limit containing at least n composites */
    uint64_t lim = 2 * (uint64_t)n + 16;
    uint64_t *spf;
    uint64_t *x = (uint64_t *)malloc((size_t)n * sizeof(uint64_t));
    for (;;) {
        spf = (uint64_t *)malloc((size_t)(lim + 1) * sizeof(uint64_t));
        a203420_build_spf(lim, spf);
        size_t len = 0;
        for (uint64_t t = 4; t <= lim && len < (size_t)n; t++)
            if (spf[t] != t) x[len++] = t;
        if (len >= (size_t)n) break;
        free(spf);
        lim *= 2;
    }

    long long *ex = (long long *)calloc((size_t)(lim + 1), sizeof(long long));

    /* numerator: Vandermonde product */
    for (idx_t j = 1; j < n; j++)
        for (idx_t i = 0; i < j; i++)
            a203420_add_factors(x[j] - x[i], spf, ex, 1);

    /* denominator: prod_{m=1}^{n-1} m^(n-m) */
    for (idx_t m = 2; m < n; m++)
        a203420_add_factors((uint64_t)m, spf, ex, -(long long)(n - m));

    seq_t r = (seq_t)1;
    for (uint64_t p = 2; p <= lim; p++) {
        for (long long e = 0; e < ex[p]; e++) r *= (seq_t)p;
    }

    free(ex);
    free(spf);
    free(x);
    return r;
}
