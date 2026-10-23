/*
 * A142125  Primes congruent to 16 mod 37.
 * https://oeis.org/A142125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   sequence of primes congruent to 16 modulo 37 (OEIS A142125)
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static const seq_t TARGET_MOD = 37;
static const seq_t TARGET_REMAINDER = 16;

/* Prime list used for trial division */
static seq_t *primes = NULL;
static size_t primes_len = 0, primes_cap = 0;
static seq_t last_generated_prime = 2;      /* last prime generated */
static seq_t next_candidate_prime = 3;      /* next odd number to test for primality */

/* Cache of sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;

/* Advances the prime list until it contains all primes <= limit */
static void extend_primes_to(seq_t limit)
{
    if (limit <= last_generated_prime)
        return;

    for (; next_candidate_prime <= limit; next_candidate_prime += 2) {
        bool prime = true;
        for (size_t i = 0; i < primes_len; ++i) {
            seq_t p = primes[i];
            if ((seq_t)p * (seq_t)p > next_candidate_prime)
                break;
            if (next_candidate_prime % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            if (primes_len == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 16;
                primes = (seq_t *)realloc(primes, primes_cap * sizeof(seq_t));
            }
            primes[primes_len++] = next_candidate_prime;
            last_generated_prime = next_candidate_prime;
        }
    }
}

/* Test primality of x using the current prime table */
static bool is_prime(seq_t x)
{
    if (x < 2)
        return false;
    seq_t sqrt_x = floor_sqrt(x);
    if (last_generated_prime < sqrt_x)
        extend_primes_to(sqrt_x);
    for (size_t i = 0; i < primes_len; ++i) {
        seq_t p = primes[i];
        if ((seq_t)p * (seq_t)p > x)
            break;
        if (x % p == 0)
            return false;
    }
    return true;
}

/* Initialize structures (runs once, before first call) */
static void init_structures(void)
{
    if (primes == NULL) {
        primes_cap = 16;
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        primes[0] = 2;
        primes_len = 1;
    }
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
}

/* Static variable to keep track of the next candidate in the 16 mod 37 progression */
static seq_t next_candidate_mod = TARGET_REMAINDER;

seq_t a(idx_t n)
{
    init_structures();
    /* Ensure we have at least n terms in the cache */
    while (cache_len < (size_t)n) {
        seq_t candidate = next_candidate_mod;
        next_candidate_mod += TARGET_MOD;           /* next number in the progression */

        if (is_prime(candidate)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
    }
    return cache[(size_t)n - 1];
}
