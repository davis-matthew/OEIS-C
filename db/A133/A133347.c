/*
 * A133347  a(n) = smallest k such that prime(n+3) = prime(n) + (prime(n) mod
 *          k), or 0 if no such k exists.
 * https://oeis.org/A133347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that prime(n+3) = prime(n) + (prime(n)
 *            mod k), or 0 if none. Indexed from n = 1
 * Range:     u8 n <= 56, u16 n <= 6547, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_first_n_primes.h"

/* Cached list of the first primes needed for evaluations. */
static uint32_t *prime_cache = NULL;
static idx_t prime_cache_size = 0;

/* Return the n-th term of A133347. */
seq_t a(idx_t n)
{
    if (n == 0)                /* indices start at 1; n=0 is out of range */
        return 0;

    /* Ensure we have primes up to position n+3 cached. */
    idx_t need = n + 3;
    if (need > prime_cache_size) {
        free(prime_cache);                /* discard old cache */
        sieve_first_n_primes(need, &prime_cache, &prime_cache_size);
    }

    /* Access primes (1‑based indices) from the cache (0‑based array). */
    uint32_t p = prime_cache[n - 1];
    uint32_t q = prime_cache[n + 3 - 1];   /* prime(n+3) */

    /* q > p for all valid n, but guard anyway. */
    if (q <= p)
        return 0;

    uint64_t pd = (uint64_t)p;
    uint64_t qd = (uint64_t)q;
    uint64_t diff = qd - pd;                /* d = q - p  */
    if (diff == 0)                          /* impossible but guard */
        return 0;

    /* x = 2p - q must be positive and divisible by k. */
    int64_t x_signed = (int64_t)(2 * pd) - (int64_t)qd;
    if (x_signed <= 0)
        return 0;
    uint64_t x = (uint64_t)x_signed;

    /* Find the smallest divisor of x that is > diff and <= p. */
    uint64_t best = 0;
    for (uint64_t i = 1; i <= x / i; ++i) {
        if (x % i == 0) {
            uint64_t j = x / i;

            if (i > diff && i <= pd && (best == 0 || i < best))
                best = i;
            if (j > diff && j <= pd && (best == 0 || j < best))
                best = j;
        }
    }

    return (best == 0) ? 0 : (seq_t)best;
}
