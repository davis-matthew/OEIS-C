/*
 * A136801  Largest prime factor of the composites in the n-th prime gap
 *          larger than 2.
 * https://oeis.org/A136801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A136801:  Largest prime factor of the composites in the n-th prime gap
 *  larger than 2.  a(1)=5, a(2)=7, …   (offset 1)
 *
 *  The function computes the term by generating primes in sequence and,
 *  for each prime gap greater than 2, scans the composites in the
 *  interval and keeps the largest prime divisor found.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* -------------------------------------------------------------------------- */
/*  Cache for previously computed a(n) values (zero‑based index)               */
/* -------------------------------------------------------------------------- */
static seq_t  *cache     = NULL;   /* a[0] … a[cache_len-1]   */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* -------------------------------------------------------------------------- */
/*  List of primes found so far (uint64_t is more than enough for the limits   */
/*  considered in this problem – up to the 10 000‑th term the prime values    */
/*  stay within 64 bits).                                                      */
/* -------------------------------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t   primes_len  = 0;
static size_t   primes_cap  = 0;

/* -------------------------------------------------------------------------- */
/*  Utility: grow the prime list array to at least `primes_cap` elements      */
/* -------------------------------------------------------------------------- */
static void grow_prime_list(void)
{
    if (primes_cap == 0) {
        primes_cap = 8;
        prime_list = (uint64_t*)realloc(prime_list, primes_cap * sizeof(uint64_t));
    } else {
        primes_cap <<= 1;
        prime_list = (uint64_t*)realloc(prime_list, primes_cap * sizeof(uint64_t));
    }
}

/* -------------------------------------------------------------------------- */
/*  Utility: grow the cache array to at least `cache_cap` elements            */
/* -------------------------------------------------------------------------- */
static void grow_cache(void)
{
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    } else {
        cache_cap <<= 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }
}

/* -------------------------------------------------------------------------- */
/*  Compute the largest prime factor of a 64‑bit composite number `n` using   */
/*  trial division by the primes already known.  The result fits in seq_t.  */
/* -------------------------------------------------------------------------- */
static seq_t largest_prime_factor_uint64(uint64_t n)
{
    if (n <= 1) return 0;
    seq_t res   = 0;
    uint64_t temp = n;

    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > temp) break;
        if (temp % p == 0) {
            res = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1) res = temp;
    return res;
}

/* -------------------------------------------------------------------------- */
/*  Build the cache of a(n) values up to index `n` (1‑based).  Uses the prime  */
/*  list and keeps track of the last prime found.  The caching works even if   */
/*  calls arrive in arbitrary order.                                         */
/* -------------------------------------------------------------------------- */
static void build_cache_upto(idx_t n)
{
    if (cache_len >= (size_t)n) return;          /* already built */

    /* Initialise the prime list with 2 if it is still empty. */
    if (primes_len == 0) {
        grow_prime_list();
        prime_list[primes_len++] = 2;
    }

    uint64_t candidate = prime_list[primes_len - 1] + 1;
    uint64_t prev_prime = prime_list[primes_len - 1];

    while (cache_len < (size_t)n) {
        /* Find the next prime. */
        while (!is_prime((idx_t)candidate)) ++candidate;

        /* Store the new prime. */
        if (primes_len == primes_cap) grow_prime_list();
        prime_list[primes_len++] = candidate;

        /* Inspect the gap between prev_prime and candidate. */
        uint64_t gap = candidate - prev_prime;
        if (gap > 2) {
            seq_t mx = 0;
            for (uint64_t m = prev_prime + 1; m < candidate; ++m) {
                if (is_prime((idx_t)m)) continue;          /* skip primes */
                seq_t lpf = largest_prime_factor_uint64(m);
                if (lpf > mx) mx = lpf;
            }
            if (cache_len == cache_cap) grow_cache();
            cache[cache_len++] = mx;
        }

        prev_prime = candidate;
        ++candidate;
    }
}

/* -------------------------------------------------------------------------- */
/*  Public entry point:  a(n) – the n‑th term of the sequence  (n>=1).        */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined but defensive */
    build_cache_upto(n);
    return cache[n - 1];              /* cache is zero‑based */
}
