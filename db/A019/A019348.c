/*
 * A019348  Primes with primitive root 21.
 * https://oeis.org/A019348
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime that has 21 as a primitive root.
   The function builds a cache of terms, generating new ones on demand.
   The algorithm tests each prime p by checking that for every prime
   divisor q of p−1, 21^((p−1)/q) ≠ 1 (mod p).  Primes dividing 21 are
   excluded implicitly, as they cannot be primitive roots. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"

static uint64_t *prime_list = NULL;   /* all primes up to a fixed limit */
static size_t prime_count = 0;        /* number of primes in prime_list */

/* cache of sequence values */
static seq_t *seq_cache = NULL;
static size_t seq_cache_size = 0;
static size_t seq_cache_cap  = 0;

/* linear increase of cache capacity */
static void enlarge_cache(void) {
    size_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    seq_cache = tmp;
    seq_cache_cap = new_cap;
}

/* Return true if x is prime, using the prebuilt prime list up to the limit. */
static bool is_prime_candidate(uint64_t x) {
    if (x < 2) return false;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return x == p;
    }
    return true;
}

/* Test whether 21 is a primitive root modulo prime p. */
static bool has_primitive_root_21(uint64_t p) {
    if (p == 2) return true;            /* 1 is the only unit mod 2 */
    if (p == 3 || p == 7) return false; /* 21 ≡ 0 (mod p) ⇒ not a unit */

    uint64_t phi = p - 1;
    uint64_t temp = phi;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t q = prime_list[i];
        if (q * q > temp) break;
        if (temp % q == 0) {
            /* remove all powers of q from temp */
            while (temp % q == 0) temp /= q;
            if (mod_pow((seq_t)21, phi / q, (seq_t)p) == (seq_t)1)
                return false;
        }
    }
    if (temp > 1) {   /* remaining prime factor */
        if (mod_pow((seq_t)21, phi / temp, (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Main access function */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out of range, return 0 */

    /* Initialize prime list once */
    if (!prime_list) {
        build_primes_upto(1000000, &prime_list, &prime_count);
    }

    /* Generate terms until we have enough */
    uint64_t candidate = 1;
    if (seq_cache_size > 0) candidate = (uint64_t)seq_cache[seq_cache_size - 1] + 1;
    while (seq_cache_size < n) {
        /* find next prime */
        do { ++candidate; } while (!is_prime_candidate(candidate));
        /* test for primitive root */
        if (has_primitive_root_21(candidate)) {
            if (seq_cache_size == seq_cache_cap) enlarge_cache();
            seq_cache[seq_cache_size++] = (seq_t)candidate;
        }
    }
    return seq_cache[n - 1];
}
