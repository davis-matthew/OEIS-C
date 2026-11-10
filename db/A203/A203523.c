/*
 * A203523  v(n)/A000178(n); v=A203521 and A000178=(superfactorials).
 * https://oeis.org/A203523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A203521(n)/A000178(n-1), A203521(n) = Product_{1<=i<j<=n}
 *            (prime(i)+prime(j)), A000178(n-1) = Product_{k=1..n-1} k!;
 *            computed by prime-exponent cancellation
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 11 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    size_t N = (size_t)n;
    /* first N primes by trial division */
    uint32_t *pr = (uint32_t *)malloc(N * sizeof(uint32_t));
    if (!pr) return (seq_t)0;
    size_t cnt = 0;
    for (uint32_t c = 2; cnt < N; ++c) {
        int ok = 1;
        for (size_t k = 0; k < cnt; ++k) {
            uint32_t q = pr[k];
            if ((uint64_t)q * q > c) break;
            if (c % q == 0) { ok = 0; break; }
        }
        if (ok) pr[cnt++] = c;
    }

    size_t L = (size_t)pr[N - 1] + (size_t)pr[N - 2] + 2;
    if (L < N + 2) L = N + 2;

    uint32_t *spf = (uint32_t *)calloc(L, sizeof(uint32_t));
    long long *ev = (long long *)calloc(L, sizeof(long long));
    if (!spf || !ev) { free(pr); free(spf); free(ev); return (seq_t)0; }

    for (size_t i = 2; i < L; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j < L; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            size_t x = (size_t)pr[i] + (size_t)pr[j];
            while (x > 1) {
                uint32_t q = spf[x];
                ev[q]++;
                x /= q;
            }
        }
    }

    /* divide by prod_{m=1}^{n-1} m^(n-m) = A000178(n-1) */
    for (size_t m = 2; m < N; ++m) {
        size_t x = m;
        while (x > 1) {
            uint32_t q = spf[x];
            ev[q] -= (long long)(N - m);
            x /= q;
        }
    }

    seq_t res = (seq_t)1;
    for (size_t q = 2; q < L; ++q) {
        for (long long e = ev[q]; e > 0; --e)
            res *= (seq_t)q;
    }

    free(pr);
    free(spf);
    free(ev);
    return res;
}
