/*
 * A130066  Primes prime(n) such that both of the numbers
 *          (prime(n+1)^2-prime(n)^2)/2 - 1 and (prime(n+1)^2-prime(n)^2)/2 +
 *          1 are primes.
 * https://oeis.org/A130066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A130066 – list of primes p for which both ((next_prime(p))^2 -
 *            p^2)/2 ± 1 are primes
 * Range:     u8 n <= 1, u16 n <= 15, u32 n <= 36, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

static uint64_t *primes = NULL;          /* array of primes */
static size_t primes_count = 0;          /* number of primes in array */
static size_t primes_limit = 0;          /* maximum value sieved */

static seq_t *qual = NULL;              /* qualifying primes (the sequence) */
static size_t qual_count = 0;            /* number of qualifying primes found */
static size_t qual_cap = 0;              /* capacity of qual array */

static size_t processed_idx = 0;         /* index of the prime pair last examined */

/* Ensure we have primes up to at least value 'limit'. Truncate & rebuild if necessary. */
static void ensure_primes(size_t limit)
{
    if (primes == NULL || limit > primes_limit)
    {
        uint64_t *new_primes = NULL;
        size_t new_count = 0;
        /* Allocate a slightly larger range to reduce rebuilds */
        size_t new_limit = limit * 2;
        if (new_limit < limit + 100000)
            new_limit = limit + 100000;
        build_primes_upto(new_limit, &new_primes, &new_count);
        if (primes) free(primes);
        primes = new_primes;
        primes_count = new_count;
        primes_limit = new_limit;
    }
}

/* Append a qualifying prime to the sequence array */
static void store_qual(seq_t p)
{
    if (qual_cap == 0)
    {
        qual_cap = 64;
        qual = (seq_t *)malloc(sizeof(seq_t) * qual_cap);
    }
    else if (qual_count >= qual_cap)
    {
        qual_cap *= 2;
        qual = (seq_t *)realloc(qual, sizeof(seq_t) * qual_cap);
    }
    qual[qual_count++] = p;
}

/* Process prime pairs until we have at least 'needed' qualifying primes. */
static void ensure_qualifies(idx_t needed)
{
    /* Use a generous initial sieve limit. */
    if (primes == NULL)
        ensure_primes(2000000); /* ~150k primes, sufficient for 10k qualifying entries */

    while (qual_count < (size_t)needed)
    {
        /* If we have exhausted the current primes array, extend the sieve. */
        if (processed_idx + 1 >= primes_count)
        {
            /* Double the sieve range and rebuild primes. */
            size_t new_limit = primes_limit * 2;
            if (new_limit < primes_limit + 100000)
                new_limit = primes_limit + 100000;
            ensure_primes(new_limit);
            /* After rebuild, attempt to continue with the same processed index. */
            if (processed_idx + 1 >= primes_count)
                break; /* safety – should not happen with sufficient limits */
        }

        uint64_t p = primes[processed_idx];
        uint64_t q = primes[processed_idx + 1];
        seq_t sq = (seq_t)q * q;
        seq_t sp = (seq_t)p * p;
        seq_t m = (sq - sp) / 2;

        if (is_prime_big(m - 1) && is_prime_big(m + 1))
            store_qual((seq_t)p);

        processed_idx++;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety – offset is 1, undefined below */
    ensure_qualifies(n);
    return qual[n - 1];
}
