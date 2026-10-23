/*
 * A177791  Partial sums of A050705.
 * https://oeis.org/A177791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = partial sum of A050705(n)
 * A050705(n) = composite n such that
 *      n + sumPrimeFactors(n) is prime and
 *      n - sumPrimeFactors(n) is prime
 * The code builds this list on demand with a simple sieve and
 * trial division.  The implementation caches results so that
 * repeated calls are O(1) after the first pass and has an
 * overall complexity of about O(m log log m) where m is the
 * largest term needed for the requested index up to ~10000.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h" /* for build_primes_upto() */
#include "../utils/is_prime.h"          /* for is_prime()          */

static seq_t *cache = NULL;          /* cached partial sums   */
static size_t cache_size = 0;        /* how many terms cached */
static size_t cache_cap = 0;         /* capacity of cache     */
static uint64_t next_candidate = 4;  /* smallest composite   */
static seq_t cumulative_sum = 0;     /* running total         */

static uint64_t *prime_list = NULL;  /* list of primes        */
static size_t prime_count = 0;       /* number of primes      */

/* initialise the global prime list once */
static void init_primes(void)
{
    if (prime_list != NULL)
        return;
    /* limit large enough for n up to 10000; 1e6 gives ~7.8e4 primes */
    const idx_t PRIME_LIMIT = 1000000;
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_count);
}

/* sum of prime factors with multiplicity */
static uint64_t sum_prime_factors(uint64_t n)
{
    uint64_t sum = 0;
    uint64_t tmp = n;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        while (tmp % p == 0) {
            sum += p;
            tmp /= p;
        }
    }
    if (tmp > 1) {
        sum += tmp;          /* remaining prime > sqrt(original n) */
    }
    return sum;
}

/* compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset starts at 1; this case is unused   */

    init_primes();

    /* extend cache until we have the desired term */
    while (cache_size < (size_t)n) {

        uint64_t m = next_candidate;

        /* find the next composite that satisfies the condition */
        for ( ; ; ++m) {
            if (m < 2 || is_prime(m))
                continue;                 /* skip primes and trivial cases */

            uint64_t sum = sum_prime_factors(m);
            if (is_prime(m + sum) && is_prime(m - sum))
                break;                    /* found a valid composite */
        }

        /* update running total and store in cache */
        cumulative_sum += (seq_t)m;

        if (cache_size >= cache_cap) {      /* grow cache if needed */
            size_t new_cap = cache_cap ? cache_cap * 2 : 64;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }
        cache[cache_size] = cumulative_sum;
        ++cache_size;

        next_candidate = m + 1;             /* next search starts after this number */
    }

    return cache[(size_t)n - 1];
}
