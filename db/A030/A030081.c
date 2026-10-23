/*
 * A030081  a(n) = prime^2 and digits of prime appear in a(n).
 * https://oeis.org/A030081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   prime_digits_in_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A030081:  
   a(n) = the square of the n‑th prime p such that every decimal digit of p
   appears somewhere in p².  The sequence is computed by searching primes
   on demand and caching the results.  The function is thread‑local static
   and works correctly for any build where seq_t holds the resulting value. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/prime_digits_in_square.h"

static seq_t   *cache      = NULL;  /* cached a(1..k) values            */
static size_t  cache_len   = 0;     /* number of cached terms            */
static size_t  cache_cap   = 0;     /* current capacity of cache array   */

static uint64_t *prime_list = NULL; /* primes discovered so far          */
static size_t  prime_len   = 0;     /* number of stored primes           */
static size_t  prime_cap   = 0;     /* capacity of prime_list            */

static uint64_t next_candidate = 2; /* next number to test for primality */

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t value)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = value;
}

/* Append a prime to the prime list, growing the list if necessary. */
static void prime_append(uint64_t p)
{
    if (prime_len == prime_cap) {
        prime_cap = prime_cap ? prime_cap * 2 : 16;
        prime_list = (uint64_t *)realloc(prime_list, prime_cap * sizeof(uint64_t));
    }
    prime_list[prime_len++] = p;
}

/* Test whether x (up to 64 bits) is prime using the primes found so far. */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1: n = 0 is unused but harmless */
    /* Fill cache until we have at least n terms */
    while (cache_len < n) {
        /* Find the next prime starting from next_candidate */
        for (uint64_t cand = next_candidate; ; ++cand) {
            /* Only check odd numbers after 2 */
            if (cand != 2 && (cand & 1) == 0) continue;
            if (is_prime_uint64(cand)) {
                /* Found a prime – process it */
                prime_append(cand);
                if (prime_digits_in_square(cand)) {
                    seq_t sq = (seq_t)cand * (seq_t)cand;
                    cache_append(sq);
                }
                next_candidate = cand + 1;
                break;  /* proceed to potentially find more primes */
            }
        }
    }
    return cache[(size_t)(n - 1)];
}
