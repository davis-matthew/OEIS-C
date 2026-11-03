/*
 * A094460  a(n) is the third term in Euclid-Mullin (EM) prime sequence
 *          initiated with n-th prime.
 * https://oeis.org/A094460
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7; for n > 1, a(n) = smallest prime divisor of (2 *
 *            prime(n) + 1)
 * Range:     u8 n <= 11, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-01)
 * Helpers:   build_primes_upto, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n <= (idx_t)1)          /* offset 1, first term */
        return (seq_t)7;

    /* Estimate an upper bound for the n‑th prime */
    idx_t limit = _prime_limit_for_idx(n);

    /* Build all primes up to that bound */
    uint64_t *primes = NULL;
    size_t   cnt   = 0;
    build_primes_upto(limit, &primes, &cnt);

    if (cnt < (size_t)n) {      /* should not happen */
        free(primes);
        return (seq_t)0;
    }

    /* n‑th prime is at index n‑1 (array is 0‑based) */
    seq_t p = (seq_t)primes[(size_t)(n - 1)];
    free(primes);

    /* Now find the smallest prime divisor of N = 2*p + 1 */
    seq_t N = (seq_t)2 * p + 1;

    if (N % (seq_t)2 == (seq_t)0)
        return (seq_t)2;

    seq_t lim = floor_sqrt(N);
    for (seq_t d = (seq_t)3; d <= lim; d += (seq_t)2) {
        if (N % d == (seq_t)0)
            return d;
    }
    /* If no divisor found, N is prime itself */
    return N;
}
