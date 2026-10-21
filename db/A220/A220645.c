/*
 * A220645  T(n,k): number of binomial coefficients C(n,r), for 0 <= r <= n,
 *          divisible by 2^k.
 * https://oeis.org/A220645
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5150, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  A220645 – Flattened triangle T(n,k) of binomial coefficients
 *  divisible by powers of 2.
 *
 *  For each n >= 0 define
 *       N = floor((sqrt(8*n + 1) - 1) / 2)          (row number)
 *       K = n - N*(N+1)/2                           (column number)
 *  Then
 *       a(n) = |{ r : 0 <= r <= N ,  v2( C(N,r) ) >= K }|
 *  where v2(x) is the exponent of 2 in the prime factorisation of x.
 *  The function below computes this value using the known
 *  popcount formula for v2 of a binomial coefficient
 *  (Kummer's theorem).
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* helper: integer square root of a 64‑bit unsigned value */
static inline unsigned long long isqrt64(unsigned long long x)
{
    unsigned long long r = sqrt((long double)x);
    while ((r + 1) * (r + 1) <= x) r++;
    while (r * r > x) r--;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Determine the row N.  N satisfies N*(N+1)/2 <= n < (N+1)*(N+2)/2. */
    unsigned long long val  = 8ULL * (unsigned long long)n + 1ULL;
    unsigned long long rt   = isqrt64(val);            /* sqrt(8n+1) */
    unsigned long long nRow = (rt - 1) / 2ULL;          /* floor((rt-1)/2) */
    idx_t N = (idx_t)nRow;

    /* 2. Compute column K. */
    idx_t start = N * (N + 1) / 2;                     /* first index of row N */
    idx_t K = n - start;

    /* 3. Count coefficients whose exponent of 2 is at least K. */
    seq_t count = 0;
    unsigned int popN = __builtin_popcountll((unsigned long long)N);

    for (idx_t r = 0; r <= N; ++r) {
        unsigned int popR  = __builtin_popcountll((unsigned long long)r);
        unsigned int popNR = __builtin_popcountll((unsigned long long)(N - r));
        unsigned int e     = popR + popNR - popN;   /* exponent of 2 in C(N,r) */
        if (e >= K) ++count;
    }

    return count;
}
