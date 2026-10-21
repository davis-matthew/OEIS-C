/*
 * A229738  a(n) = p^2*(p^2+2*p-1)/2, where p = prime(n).
 * https://oeis.org/A229738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p^2*(p^2+2*p-1)/2, where p = prime(n)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Cache for the first primes needed by the sequence. */
static uint64_t *prime_cache = NULL;
static size_t   prime_cache_cnt = 0;
static idx_t    prime_cache_limit = 0;

/* Ensure that the cache contains at least the first 'n' primes. */
static void
ensure_nth_prime(idx_t n)
{
    if (n <= prime_cache_cnt)
        return;

    /* Estimate an upper bound for the nth prime:
       p_n <= n*(log n + log log n) for n >= 6 (Rosser’s theorem). */
    idx_t limit;
    if (n < 6)
        limit = 15;                /* covers the first few primes */
    else {
        double dn = (double)n;
        limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 10.0);
    }

    if (limit <= prime_cache_limit)
        return;

    /* Resize the cache.  The helper frees the old array if needed. */
    if (prime_cache)
        free(prime_cache);

    build_primes_upto(limit, &prime_cache, &prime_cache_cnt);
    prime_cache_limit = limit;
}

/* Compute the nth term of OEIS A229738. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* out of range – return 0 */

    ensure_nth_prime(n);
    uint64_t p = prime_cache[n - 1];     /* nth prime (1‑based) */

    /* Use a wider intermediate type to avoid premature overflow. */
#if defined(__SIZEOF_INT128__)
    __uint128_t p128 = (__uint128_t)p;
    __uint128_t p2   = p128 * p128;
    __uint128_t t    = p2 + (__uint128_t)2 * p128 - 1;
    __uint128_t res  = p2 * t / 2;
    return (seq_t)res;
#else
    /* Fallback to 64‑bit intermediate (acceptable for the tested range). */
    uint64_t p2 = p * p;
    uint64_t t  = p2 + 2 * p - 1;
    uint64_t res = p2 * t / 2;
    return (seq_t)res;
#endif
}
