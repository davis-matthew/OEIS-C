/*
 * A235985  Primes p such that 3p-1 has even Hamming weight.
 * https://oeis.org/A235985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   seq_popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by search: find the nth prime p such that popcount(3p-1) is even. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_popcount.h"

/* Static cache of primes satisfying the property. */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;
static size_t cache_cap    = 0;

/* List of primes found so far, used to test further candidates. */
static uint64_t *prime_list = NULL;
static size_t  prime_len    = 0;
static size_t  prime_cap    = 0;
static uint64_t next_candidate = 2; /* next number to test for primality */

/* Ensure cache can hold at least 'need' elements. */
static void grow_cache(size_t need)
{
    if (cache_cap < need) {
        size_t ncap = cache_cap ? cache_cap << 1 : 16;
        if (ncap < need) ncap = need;
        cache = (seq_t *)realloc(cache, ncap * sizeof(seq_t));
        cache_cap = ncap;
    }
}

/* Ensure prime list can hold at least 'need' elements. */
static void grow_prime_list(size_t need)
{
    if (prime_cap < need) {
        size_t ncap = prime_cap ? prime_cap << 1 : 16;
        if (ncap < need) ncap = need;
        prime_list = (uint64_t *)realloc(prime_list, ncap * sizeof(uint64_t));
        prime_cap = ncap;
    }
}

/* Test whether a uint64_t number is prime using trial division with
   the prime list we have accumulated.  The list is assumed to be
   sorted in ascending order. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Generate primes and cache until we have at least 'n' desired terms. */
static void ensure_n_terms(idx_t n)
{
    while (cache_len < (size_t)n) {
        /* Test the current candidate for primality. */
        if (is_prime_uint64(next_candidate)) {
            /* New prime found. */
            grow_prime_list(prime_len + 1);
            prime_list[prime_len++] = next_candidate;

            seq_t val = (seq_t)next_candidate;
            seq_t num = 3 * val - 1;
            unsigned pc = seq_popcount(num);
            if ((pc & 1) == 0) {            /* even Hamming weight */
                grow_cache(cache_len + 1);
                cache[cache_len++] = val;
            }
        }
        ++next_candidate;
    }
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined but harmless fallback */
    ensure_n_terms(n);
    return cache[n - 1];
}
