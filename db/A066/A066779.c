/*
 * A066779  Sum of squarefree numbers <= n.
 * https://oeis.org/A066779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d=1..⌊√n⌋} μ(d) * d² * (⌊n/d²⌋ * (⌊n/d²⌋ + 1) / 2)
 * Range:     u8 n <= 29, u16 n <= 462, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where μ is the Möbius function and μ(1)=1, μ(p)=-1 for primes, μ(n)=0 if n contains a squared prime. */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* Integer square root: floor(√x) for unsigned 64‑bit x using a long double estimate.
   The estimates are corrected by a few bumps so that the result is guaranteed to be exact. */
static inline idx_t isqrt_idx(idx_t x)
{
    if (x <= 1) return x;
    idx_t r = (idx_t)sqrt((long double)x);
    /* Correct for possible rounding errors */
    while ((uint64_t)(r + 1) * (r + 1) <= x) ++r;
    while ((uint64_t)r * r > x) --r;
    return r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t m = isqrt_idx(n);                 /* upper bound for μ indices   */
    if (m == 0) return (seq_t)0;

    /* Allocate small helpers; calloc zero‑initialises them */
    int8_t  *mu     = (int8_t *)calloc((size_t)(m + 1), sizeof(int8_t));
    int     *is_comp= (int *)calloc((size_t)(m + 1), sizeof(int));
    int     *prime  = (int *)calloc((size_t)(m + 1), sizeof(int));
    if (!mu || !is_comp || !prime) {       /* out of memory – return 0 */
        free(mu); free(is_comp); free(prime);
        return (seq_t)0;
    }

    /* Linear sieve to compute μ up to m */
    int primes_cnt = 0;
    mu[1] = 1;   /* μ(1) = 1 */
    for (idx_t i = 2; i <= m; ++i) {
        if (!is_comp[i]) {
            prime[primes_cnt++] = (int)i;
            mu[i] = -1;   /* prime */
        }
        for (int j = 0; j < primes_cnt; ++j) {
            idx_t p = (idx_t)prime[j];
            idx_t v = i * p;
            if (v > m) break;
            is_comp[v] = 1;
            if (i % p == 0) {
                mu[v] = 0;          /* square factor found */
                break;
            } else {
                mu[v] = -mu[i];     /* μ(i)*(-1) */
            }
        }
    }

    /* Accumulate the sum using the Möbius‑summation formula */
    __int128 total = 0;
    for (idx_t d = 1; d <= m; ++d) {
        if (mu[d] == 0) continue;
        __int128 d2  = (__int128)d * (__int128)d;        /* d²                      */
        __int128 t   = (__int128)n / d2;                 /* ⌊n / d²⌋                */
        __int128 add = (__int128)mu[d] * d2 * t * (t + 1) / 2;
        total += add;
    }

    free(mu); free(is_comp); free(prime);
    return (seq_t)total;
}
