/*
 * A072666  Numbers n such that prime(n) + prime(n+1) - 1 is prime.
 * https://oeis.org/A072666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth index i such that prime(i) + prime(i+1) – 1 is
 *            prime * * The function builds a prime sieve large enough to
 *            generate the first `n` * qualifying indices. It uses the helper
 *            estimate_prime_upper(…) from * "../utils/estimate_prime_upper.h"
 *            to compute a good upper bound for the * nth pr...
 * Range:     u8 n <= 15, u16 n <= 911, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   estimate_prime_upper
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/estimate_prime_upper.h"

static size_t  sieve_lim        = 0;      /* last used sieve limit (inclusive)      */
static uint8_t *is_prime_sieve  = NULL;   /* stride 1 bit array                       */
static seq_t  *prime_list       = NULL;   /* primes up to sieve_lim                    */
static size_t  prime_count      = 0;      /* number of primes found                    */
static seq_t  *qual_indices     = NULL;   /* list of qualifying index values           */
static size_t  qual_count       = 0;      /* number of qualifying indices found        */

/* Expand the sieve and the cached data until at least `need_n` qualifying
 * indices have been found.  The function is called from *a* if the current
 * cache does not contain enough values.  It is tolerant to multiple calls
 * with increasing `need_n`. */
static void
ensure_cache(idx_t need_n)
{
    /* Quick exit if already satisfied */
    if (qual_count >= (size_t)need_n)
        return;

    /* Start with a conservative estimate of how many indices must be
     * examined.  The factor 15 gives enough room for the density of the
     * primes condition.  The value `+200` protects against very small n.
     */
    size_t idx_limit = (size_t)need_n * 15 + 200;
    if (idx_limit < 100)
        idx_limit = 100;

    /* Repeat until we have enough primes and enough qualifying indices. */
    while (1)
    {
        /* Compute an upper bound for the (idx_limit+5)-th prime. */
        size_t prime_upper = estimate_prime_upper((uint64_t)(idx_limit + 5));

        /* Sieve up to a number large enough to hold the sum of two such
         * primes.  Adding 500 for safety keeps the sieve size manageable. */
        size_t new_sieve_lim = prime_upper * 2 + 500;
        if (new_sieve_lim < 100000)          /* at least a minimal size */
            new_sieve_lim = 100000;

        /* Reallocate the sieve array. */
        if (is_prime_sieve)
            free(is_prime_sieve);
        is_prime_sieve = (uint8_t *)calloc(new_sieve_lim + 1, 1);
        if (!is_prime_sieve)
            return; /* allocation failed, caller will get 0 */
        sieve_lim = new_sieve_lim;

        /* Classic Eratosthenes sieve. */
        for (size_t i = 2; i <= new_sieve_lim; ++i)
            is_prime_sieve[i] = 1;
        is_prime_sieve[0] = is_prime_sieve[1] = 0;
        for (size_t p = 2; p * p <= new_sieve_lim; ++p)
        {
            if (!is_prime_sieve[p])
                continue;
            for (size_t j = p * p; j <= new_sieve_lim; j += p)
                is_prime_sieve[j] = 0;
        }

        /* Build the prime list.  We need at least idx_limit+2 primes (for
         * prime(idx_limit+1)).  Allocate slightly more memory than the
         * expected number of primes to avoid reallocations. */
        if (prime_list)
            free(prime_list);
        prime_list = (seq_t *)malloc(sizeof(seq_t) * (idx_limit + 10));
        if (!prime_list)
            return; /* allocation failed */

        prime_count = 0;
        for (size_t i = 2; i <= new_sieve_lim; ++i)
        {
            if (is_prime_sieve[i])
            {
                if (prime_count < idx_limit + 10)
                    prime_list[prime_count++] = (seq_t)i;
            }
        }

        /* If we did not obtain enough primes, double the search range and
         * restart the sieve. */
        if (prime_count < idx_limit + 2)
        {
            idx_limit *= 2;
            continue;
        }

        /* Now find all qualifying indices up to idx_limit. */
        if (qual_indices)
            free(qual_indices);
        qual_indices = (seq_t *)malloc(sizeof(seq_t) * (idx_limit + 10));
        if (!qual_indices)
            return;
        qual_count = 0;
        for (size_t i = 1; i <= idx_limit; ++i)
        {
            if ((size_t)i >= prime_count)
                break;                   /* no p_{i+1} available */

            seq_t p1 = prime_list[i - 1];
            seq_t p2 = prime_list[i];      /* prime(i+1) */
            seq_t sum = p1 + p2 - 1;

            if ((size_t)sum <= new_sieve_lim && is_prime_sieve[(size_t)sum])
            {
                if (qual_count < idx_limit + 10)
                    qual_indices[qual_count++] = (seq_t)i;
            }
        }

        /* If we still haven't reached the required number of terms,
         * increase the examined index span and repeat. */
        if (qual_count >= (size_t)need_n)
            break;
        idx_limit *= 2;
    }
}

/* --------------------------------------------------------------------
 *  Public function requested by the problem statement.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0; /* undefined, but safe return */

    ensure_cache(n);
    if ((size_t)n-1 >= qual_count)
        return (seq_t)0; /* safety out of bounds */

    return qual_indices[(size_t)n - 1];
}
