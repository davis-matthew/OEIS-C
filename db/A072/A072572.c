/*
 * A072572  Odd interprimes divisible by 3.
 * https://oeis.org/A072572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = average of two consecutive odd primes that is odd and
 *            divisible by 3
 * Range:     u8 n <= 16, u16 n <= 1869, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

/* seq_t and idx_t are defined by the build environment */

static uint64_t *primes      = NULL;   /* buffer of odd primes */
static size_t   primes_cnt   = 0;      /* number of primes stored */
static size_t   primes_cap   = 0;      /* current capacity of primes buffer */
static uint64_t next_candidate = 3;    /* next odd number to test for primality */

static seq_t *cache       = NULL;   /* sequence values buffer */
static size_t  cache_cnt  = 0;      /* number of values stored */
static size_t  cache_cap  = 0;      /* current capacity of cache buffer */

static uint64_t processed_index = 0; /* index of the first prime in the next pair */

/* Grow primes buffer to at least the requested capacity */
static void ensure_primes_capacity(size_t needed)
{
    if (primes_cap < needed)
    {
        size_t new_cap = primes_cap ? primes_cap << 1 : 128;
        while (new_cap < needed) new_cap <<= 1;
        primes = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        primes_cap = new_cap;
    }
}

/* Grow cache buffer to at least the requested capacity */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap < needed)
    {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        while (new_cap < needed) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Generate next odd prime and store it */
static void generate_next_prime(void)
{
    while (true)
    {
        uint64_t cand = next_candidate;
        next_candidate += 2;  /* skip even numbers */
        if (is_prime((idx_t)cand))
        {
            ensure_primes_capacity(primes_cnt + 1);
            primes[primes_cnt++] = cand;
            break;
        }
    }
}

/* Ensure that primes[processed_index] and primes[processed_index+1] exist */
static void ensure_next_pair(void)
{
    while (primes_cnt <= processed_index + 1)
        generate_next_prime();
}

/* Return the n-th odd interprime divisible by 3 (1‑based) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    size_t target = (size_t)n;

    while (cache_cnt < target)
    {
        /* make sure we have the next pair of primes */
        ensure_next_pair();

        uint64_t p1 = primes[processed_index];
        uint64_t p2 = primes[processed_index + 1];
        uint64_t avg = (p1 + p2) >> 1;   /* (p1+p2)/2 */

        if ((avg & 1ULL) && (avg % 3ULL == 0ULL))
        {
            ensure_cache_capacity(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)avg;
        }

        processed_index++;  /* move to the next pair */
    }

    return cache[target - 1];
}
