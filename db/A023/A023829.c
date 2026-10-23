/*
 * A023829  Sum of exponents in prime-power factorization of C(4n,n-3).
 * https://oeis.org/A023829
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = bigomega(C(4n, n-3)) = Σ_p (⌊4n/p⌋ + ⌊4n/p²⌋ + …) –
 *            (⌊(n−3)/p⌋ + ⌊(n−3)/p²⌋ + …) – (⌊(3n+3)/p⌋ + ⌊(3n+3)/p²⌋ + …)
 *            over all primes p ≤ 4n
 * Range:     u8 n <= 749, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cached prime list */
static uint64_t *g_primes = NULL;     /* primes up to g_prime_limit */
static size_t   g_prime_cnt = 0;
static idx_t    g_prime_limit = 0;

/* Ensure primes up to the given limit */
static void
ensure_primes(idx_t limit)
{
    if (limit > g_prime_limit) {
        if (g_primes) free(g_primes);
        build_primes_upto(limit, &g_primes, &g_prime_cnt);
        g_prime_limit = limit;
    }
}

/* Legendre exponent of prime p in m! */
static inline idx_t
exp_in_fact(idx_t m, idx_t p)
{
    idx_t sum = 0;
    idx_t div = p;
    while (div <= m) {
        sum += m / div;
        if (div > m / p) break;          /* prevent overflow of div *= p */
        div *= p;
    }
    return sum;
}

/* External function required by the test harness */
seq_t
a(idx_t n)
{
    if (n < 3) return (seq_t)0;           /* undefined for n < 3 */

    idx_t four_n      = 4 * n;
    idx_t n_minus_3   = n - 3;
    idx_t three_n_plus_3 = 3 * n + 3;

    /* Make sure we have all primes needed for the factorisation. */
    ensure_primes(four_n);

    seq_t total = (seq_t)0;

    for (size_t i = 0; i < g_prime_cnt; ++i) {
        idx_t p = (idx_t)g_primes[i];
        if (p > four_n) break;

        idx_t e = exp_in_fact(four_n, p)
                - exp_in_fact(n_minus_3, p)
                - exp_in_fact(three_n_plus_3, p);

        if (e)
            total += (seq_t)e;
    }

    return total;
}
