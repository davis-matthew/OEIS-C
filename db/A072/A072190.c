/*
 * A072190  Indices of primes with primitive root 2.
 * https://oeis.org/A072190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 77 terms through n = 967 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: indices of primes with primitive root 2.
   Computed by scanning primes and testing the primitive root condition. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

/* Simple Sieve of Eratosthenes up to LIMIT (inclusive). */
static void build_primes_upto_uint64(uint64_t limit, uint64_t **out, size_t *cnt_out) {
    bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }
    size_t count = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++count;
    uint64_t *pr = (uint64_t *)malloc(count * sizeof(uint64_t));
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) pr[idx++] = i;
    *out = pr;
    *cnt_out = count;
    free(is_prime);
}

/* Modular exponentiation for 64‑bit modulus (used for prime checks). */
static inline seq_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    seq_t result = mod ? 1 % mod : 0;
    seq_t b = mod ? base % mod : 0;
    while (exp > 0) {
        if (exp & 1U)
            result = (seq_t)((unsigned __int128)result * b % mod);
        b = (seq_t)((unsigned __int128)b * b % mod);
        exp >>= 1U;
    }
    return result;
}

/* Test whether 2 is a primitive root modulo a prime p. */
static bool two_is_primitive_root(uint64_t p, const uint64_t *primes, size_t prime_count) {
    if (p == 2) return true;             /* not used for the sequence */
    uint64_t phi = p - 1;
    uint64_t n = phi;
    for (size_t i = 0; i < prime_count && primes[i] * primes[i] <= n; ++i) {
        uint64_t q = primes[i];
        if (n % q == 0) {
            if (mod_pow_uint64(2, phi / q, p) == 1U)
                return false;
            while (n % q == 0) n /= q;
        }
    }
    if (n > 1) {                         /* remaining prime factor */
        if (mod_pow_uint64(2, phi / n, p) == 1U)
            return false;
    }
    return true;
}

/* Static state for the sequence. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static seq_t *index_list = NULL;
static size_t indices_used = 0;
static size_t indices_capacity = 0;
static size_t prime_scan_pos = 0;        /* index in prime_list to examine next */
static bool initialized = false;

/* Initialise static data. */
static void init_sequence(void) {
    const uint64_t LIMIT = 5000000ULL;   /* enough for n up to at least 10 000 */
    build_primes_upto_uint64(LIMIT, &prime_list, &prime_count);

    indices_capacity = prime_count;     /* safe upper bound */
    index_list = mem_new(seq_t, indices_capacity);
    indices_used = 0;

    prime_scan_pos = 1;                 /* start from the prime 3 (index 2) */
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;        /* undefined, return 0 as safe fallback */

    if (!initialized) init_sequence();

    /* Generate indices until we have at least n of them. */
    while (indices_used < (size_t)n) {
        if (prime_scan_pos >= prime_count) return (seq_t)0; /* safety fallback */

        uint64_t p = prime_list[prime_scan_pos];
        if (two_is_primitive_root(p, prime_list, prime_count))
            index_list[indices_used++] = (seq_t)(prime_scan_pos + 1); /* 1‑based index */

        ++prime_scan_pos;
    }
    return index_list[n - 1];
}
