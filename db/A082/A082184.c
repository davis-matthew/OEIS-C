/*
 * A082184  The a(n)-th triangular number is the sum of the n-th triangular
 *          number and the smallest triangular number possible.
 * https://oeis.org/A082184
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index m of the first triangular number * T_m that
 *            can be written as the sum of the n-th triangular number * T_n
 *            and the smallest triangular number possible
 * Range:     u8 n <= 30, u16 n <= 3390, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 83 terms through n = 8955 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A082184: the least m > n with T(m) - T(n) a positive triangular number T(k).
 * With x = 2m+1, y = 2n+1, z = 2k+1 the condition is (x - z)(x + z) = y^2 - 1
 * = 4n(n+1); x - z and x + z are even, so x = u + v and z = v - u for a
 * factorization u v = n(n+1) with u + v odd. u + n(n+1)/u falls as u grows to
 * sqrt(n(n+1)), and u = n gives x = y (m = n), so the least m comes from the
 * largest divisor u < n of n(n+1) with u + n(n+1)/u odd and v - u >= 3 (k >= 1):
 * m = (u + v - 1)/2. (The original tried every m and overflowed near 2^31.) */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 u128;

static int factor(uint64_t x, uint64_t *P, int *E, int np)  /* adds x's primes */
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p) continue;
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }
        int i = 0;
        while (i < np && P[i] != p) ++i;
        if (i == np) { P[np] = p; E[np++] = 0; }
        E[i] += e;
    }
    if (x > 1) {
        int i = 0;
        while (i < np && P[i] != x) ++i;
        if (i == np) { P[np] = x; E[np++] = 0; }
        E[i] += 1;
    }
    return np;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;
    const uint64_t N = (uint64_t)n;
    uint64_t P[32];
    int E[32], np = factor(N, P, E, 0);
    np = factor(N + 1, P, E, np);
    const u128 M = (u128)N * (N + 1);
    /* every divisor u < n of M: keep the best */
    size_t cnt = 1;
    for (int i = 0; i < np; ++i) cnt *= (size_t)(E[i] + 1);
    uint64_t *d = (uint64_t *)malloc(cnt * sizeof *d);
    if (!d) abort();
    size_t nd = 1;
    d[0] = 1;
    for (int i = 0; i < np; ++i) {
        size_t old = nd;
        u128 m = 1;
        for (int e = 1; e <= E[i]; ++e) {
            m *= P[i];
            for (size_t j = 0; j < old; ++j) {
                u128 v = (u128)d[j] * m;
                if (v < N) d[nd++] = (uint64_t)v;      /* only u < n matters */
            }
        }
    }
    uint64_t best = 0;
    for (size_t j = 0; j < nd; ++j) {
        u128 u = d[j], v = M / u;
        if (((u + v) & 1) && v - u >= 3 && u > best) best = (uint64_t)u;
    }
    free(d);
    if (!best) return (seq_t)0;
    u128 x = (u128)best + M / best;
    return (seq_t)(uint64_t)((x - 1) / 2);
}
