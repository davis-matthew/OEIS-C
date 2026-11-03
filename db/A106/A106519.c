/*
 * A106519  a(n) = (2/n)*binomial(2*n-2, n-1) - (1/(2*n))*Sum_{d|n}
 *          Moebius(d)*binomial(2*n/d, n/d).
 * https://oeis.org/A106519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2/n)*binomial(2*n-2, n-1) - (1/(2*n))*Sum_{d|n}
 *            Moebius(d)*binomial(2*n/d, n/d)
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 40, u128 n <= 72,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 936 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A106519: a(n) = (2/n) C(2n-2, n-1) - (1/(2n)) Sum_{d|n} Moebius(d) C(2n/d, n/d).
 * The binomials and the sum are formed in 128 bits at the fixed widths (in
 * seq_t for big) and divided exactly, so a(n) is right whenever it fits (the
 * original divided values that had wrapped). */

#include <stdint.h>

#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __int128 wide_t;                  /* exact while the binomials fit in 127 bits */
#endif

static wide_t binom_w(uint64_t n, uint64_t k)
{
    wide_t r = 1;
    if (k > n - k) k = n - k;
    for (uint64_t i = 1; i <= k; ++i) r = r * (wide_t)(n - k + i) / (wide_t)i;   /* exact */
    return r;
}

static int weight(uint64_t d)                 /* Moebius(d) */
{
    int mu = 1;
    for (uint64_t p = 2; p * p <= d; ++p) {
        if (d % p) continue;
        d /= p;
        if (d % p == 0) return 0;
        mu = -mu;
    }
    return d > 1 ? -mu : mu;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    wide_t s = 0;
    for (uint64_t d = 1; d <= N; ++d)
        if (N % d == 0) s += (wide_t)weight(d) * binom_w(2 * N / d, N / d);
    wide_t r = (wide_t)2 * binom_w(2 * N - 2, N - 1) / (wide_t)N - s / (wide_t)(2 * N);
    return (seq_t)r;
}
