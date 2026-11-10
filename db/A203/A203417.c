/*
 * A203417  a(n) = A203415(n)/A000178(n).
 * https://oeis.org/A203417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A203415(n)/A000178(n-1) = (Product_{1<=i<j<=n}
 *            (c(j)-c(i))) / (Product_{k=1..n-1} k!), where c(k) is the k-th
 *            nonprime (A018252: 1, 4, 6, 8, 9, ...). Computed exactly as a
 *            prime-exponent vector: add the exponents of every difference,
 *            subtract the exponents of the superfactorial, then multi...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 14, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 129 (2026-11-07)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    size_t N = (size_t)n;
    uint64_t *x = (uint64_t *)malloc(N * sizeof(uint64_t));
    if (!x) abort();
    uint64_t c = 1;
    x[0] = 1;
    for (size_t k = 1; k < N; ++k) {
        do { ++c; } while (is_prime((idx_t)c));
        x[k] = c;
    }
    uint64_t maxv = x[N - 1];
    if (maxv < (uint64_t)N) maxv = (uint64_t)N;
    size_t L = (size_t)maxv + 1;

    /* smallest prime factor sieve */
    uint64_t *spf = (uint64_t *)calloc(L, sizeof(uint64_t));
    long long *ex = (long long *)calloc(L, sizeof(long long));
    if (!spf || !ex) abort();
    for (size_t i = 2; i < L; ++i) {
        if (spf[i] == 0) {
            for (size_t j = i; j < L; j += i)
                if (spf[j] == 0) spf[j] = (uint64_t)i;
        }
    }

    /* Vandermonde differences */
    for (size_t j = 1; j < N; ++j) {
        for (size_t i = 0; i < j; ++i) {
            uint64_t d = x[j] - x[i];
            while (d > 1) {
                uint64_t p = spf[d];
                d /= p;
                ex[p]++;
            }
        }
    }

    /* divide by 1!*2!*...*(n-1)!: integer m (2<=m<=n-1) appears in k! for k=m..n-1, i.e. n-m times */
    for (size_t m = 2; m < N; ++m) {
        long long times = (long long)(N - m);
        uint64_t d = (uint64_t)m;
        while (d > 1) {
            uint64_t p = spf[d];
            d /= p;
            ex[p] -= times;
        }
    }

    seq_t res = (seq_t)1;
    for (size_t p = 2; p < L; ++p) {
        long long e = ex[p];
        for (long long t = 0; t < e; ++t) res *= (seq_t)p;
    }

    free(x);
    free(spf);
    free(ex);
    return res;
}
