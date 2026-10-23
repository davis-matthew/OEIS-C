/*
 * A040159  Primes p such that x^5 = 2 has a solution mod p.
 * https://oeis.org/A040159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A040159: primes p such that the congruence x^5 = 2 (mod p) has a solution.
 * Computed by iterating over primes and testing the condition directly.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* dynamic array for the sequence values */
static seq_t *cache      = NULL;
static size_t cache_cnt  = 0;
static size_t cache_cap  = 0;

/* dynamic array of primes found so far */
static uint64_t *primes  = NULL;
static size_t primes_cnt = 0;
static size_t primes_cap = 0;

/* next candidate to test for primality */
static uint64_t next_candidate = 2;

/* append a new seq_t value to the cache */
static void append_cache(seq_t val)
{
    if (cache_cnt == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_cnt++] = val;
}

/* append a new prime to the prime list */
static void append_prime(uint64_t p)
{
    if (primes_cnt == primes_cap) {
        primes_cap = primes_cap ? primes_cap * 2 : 16;
        primes = (uint64_t *)realloc(primes, primes_cap * sizeof(uint64_t));
    }
    primes[primes_cnt++] = p;
}

/* test whether a prime p satisfies the congruence */
static bool qualifies(uint64_t p)
{
    if (p == 2) return true;
    if ((p - 1) % 5 != 0) return true;      /* p ≠ 1 (mod 5) ⇒ always solvable */
    seq_t pow_val = mod_pow((seq_t)2, (p - 1) / 5, (seq_t)p);
    return pow_val == (seq_t)1;
}

/* obtain the next prime after the current candidate */
static uint64_t next_prime(void)
{
    if (next_candidate == 2) {
        uint64_t p = 2;
        next_candidate = 3;
        return p;
    }

    for (;;) {
        uint64_t cand = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= cand; ++i) {
            if (cand % primes[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            next_candidate += 2;   /* skip to the next odd candidate */
            return cand;
        }
        next_candidate += 2;
    }
}

/* OEIS entry: a(idx_t n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1; undefined for n = 0 */

    while (cache_cnt < (size_t)n) {
        uint64_t p = next_prime();
        append_prime(p);
        if (qualifies(p))
            append_cache((seq_t)p);
    }
    return cache[(size_t)n - 1];
}
