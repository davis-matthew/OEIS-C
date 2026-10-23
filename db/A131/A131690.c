/*
 * A131690  a(n) = Product_{k=1..n} A008578(k)^A008578(n-k+1).
 * https://oeis.org/A131690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} A008578(k)^A008578(n-k+1) where A008578 is the
 *            sequence 1,2,3,5,7,11,... (primes with a leading 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   _prime_limit_for_idx, big_pow, build_primes_upto, seq_mul,
 *            seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility functions provided by the build environment */
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_one.h"
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* Static cache of primes (starting at 2).  We keep the largest
   prime list needed across calls so repeated invocations are fast. */
static uint64_t *prime_cache = NULL;
static size_t prime_count = 0;
static idx_t cache_n_needed = 0;

/* Build or grow the prime cache so that it contains at least
   (n-1) primes – the index 0 element of A008578 is 1, so the first
   actual prime is A008578(2). */
static void ensure_prime_cache(idx_t n)
{
    if (n <= cache_n_needed)
        return;                           /* already large enough */

    /* Free any old cache */
    if (prime_cache != NULL) {
        free(prime_cache);
        prime_cache = NULL;
    }

    /* We need primes up to the (n-1)-th prime.  The helper
       _prime_limit_for_idx gives an upper bound that contains
       at least that many primes. */
    idx_t needed_primes = (n > 1 ? n - 1 : 1);
    idx_t limit = _prime_limit_for_idx(needed_primes);

    build_primes_upto(limit, &prime_cache, &prime_count);

    cache_n_needed = n;
}

/* Top‑level sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return seq_one();                /* for indices below the offset */

    /* Ensure we have enough primes for the current n. */
    ensure_prime_cache(n);

    seq_t result = seq_one();           /* start with 1 */

    /* Loop over k = 2 .. n, skipping k = 1 because A008578(1) = 1
       contributes nothing to the product. */
    for (idx_t k = 2; k <= n; ++k)
    {
        /* Base = A008578(k) = prime_{k-1} (1-indexed primes) */
        seq_t base = (seq_t)prime_cache[k - 2];

        /* Exponent = A008578(n-k+1).  If rev == 1 then exponent = 1. */
        idx_t rev = n - k + 1;
        seq_t exp;
        if (rev == 1)
        {
            exp = seq_one();
        }
        else
        {
            exp = (seq_t)prime_cache[rev - 2];
        }

        /* If exponent == 1, just multiply by the base once. */
        if (exp == seq_one())
        {
            result = seq_mul(result, base);
            continue;
        }

        /* General case: multiply by base^exp.  big_pow handles exponentiation. */
        seq_t power = big_pow(base, exp);
        result = seq_mul(result, power);
    }

    return result;
}
