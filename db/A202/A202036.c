/*
 * A202036  Smallest prime residue of x^n (mod n) for x=0..n-1, or 0 if no
 *          such prime exists.
 * https://oeis.org/A202036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime residue of x^n (mod n) for x = 0..n-1, or
 *            0 if none. Algorithm: For each n, compute k^n mod n for all k,
 *            check whether the residue is prime using a simple sieve memoized
 *            up to the current maximum n; return the smallest such prime
 * Range:     u8 n <= 461, u16 n <= 8287, u32 n <= 8287, u64 n <= 8287,
 *            u128 n <= 8287, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

/* Cached prime sieve up to prime_cache_max (inclusive). */
static bool *prime_flags = NULL;
static idx_t prime_cache_max = 0;

/* Ensure that prime_flags is allocated and contains the primality
   status for integers 0..limit.  The sieve is rebuilt from scratch
   whenever a larger limit is requested (sufficiently fast for n <= 10_000). */
static void ensure_prime_sieve(idx_t limit)
{
    if (limit <= prime_cache_max)
        return;

    /* Allocate a new boolean array. */
    bool *flags = (bool *)malloc((size_t)(limit + 1) * sizeof(bool));
    if (!flags)
        return;  /* allocation failure: subsequent calls will keep old sieve */

    for (idx_t i = 0; i <= limit; ++i)
        flags[i] = true;

    if (limit >= 0) flags[0] = false;
    if (limit >= 1) flags[1] = false;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (flags[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                flags[j] = false;
        }
    }

    free(prime_flags);
    prime_flags = flags;
    prime_cache_max = limit;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;  /* no primes can appear as residues */

    /* Make sure we have a sieve covering at least 0..n-1. */
    ensure_prime_sieve(n - 1);

    seq_t min_prime = 0;

    for (idx_t x = 0; x < n; ++x) {
        seq_t r = mod_pow((seq_t)x, n, (seq_t)n);  /* compute x^n mod n */

        if (r < 2)
            continue;  /* 0 or 1 cannot be prime */

        /* r < n, so it fits into idx_t and is inside the sieve. */
        idx_t r_int = (idx_t)r;
        if (r_int <= prime_cache_max && prime_flags[(size_t)r_int]) {
            if (min_prime == 0 || r < min_prime) {
                min_prime = r;
                if (min_prime == 2)
                    break;  /* cannot find a smaller prime */
            }
        }
    }

    return min_prime;  /* 0 if none found */
}
