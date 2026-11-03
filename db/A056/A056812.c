/*
 * A056812  Number of unitary prime factors of lcm[1..n], i.e., primes in LCM
 *          with exponent 1.
 * https://oeis.org/A056812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = primepi(n) - primepi(floor(sqrt(n))) * (number of primes
 *            p with p ≤ n < p²)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 99 terms through n = 978 (2026-11-01)
 * Helpers:   build_primes, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>                     /* for free() */
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/isqrt.h"

/* Static cache of primes up to the largest n seen */
static idx_t *primes = NULL;
static size_t prime_count = 0;
static idx_t max_cached = 0;

/* Count how many primes in the cache are ≤ v */
static size_t count_leq(idx_t v)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_count; ++i)
    {
        if (primes[i] <= v)
            ++cnt;
        else
            break;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1)
        return (seq_t)0;

    /* Extend prime cache if needed */
    if (n > max_cached)
    {
        free(primes);
        build_primes(n, &primes, &prime_count);
        max_cached = n;
    }

    idx_t sq = isqrt(n);
    size_t cnt_n  = count_leq(n);
    size_t cnt_sq = count_leq(sq);

    return (seq_t)(cnt_n - cnt_sq);
}
