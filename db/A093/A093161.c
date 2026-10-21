/*
 * A093161  Even integers k such that there exists a prime p with p = min{q: q
 *          prime and (k - q) prime} and (k - p) < p^3.
 * https://oeis.org/A093161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the n-th even integer k for which the minimal prime p
 *            with (k - p) prime satisfies (k - p) < p^3
 * Range:     u8 n <= 65, u16 n <= 4045, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Necessary includes and utilities
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   // isPrimeGeneric(seq_t n)

#define PRIME_SMALL_LIMIT 5000000   // limit for quick small prime checks

// Global state for primes and cache
static uint64_t *primes = NULL;        // list of primes up to PRIME_SMALL_LIMIT
static size_t prime_cnt = 0;
static bool *prime_small = NULL;       // sieve for primes <= PRIME_SMALL_LIMIT

static seq_t *seq_cache = NULL;        // cache of computed sequence terms
static size_t seq_cap = 0;
static size_t seq_size = 0;
static seq_t last_k = (seq_t)2;        // last even number tested

// ---------------------------------------------------------------------
// Helper: initialize primes up to PRIME_SMALL_LIMIT
static void init_primes(void)
{
    if (primes) return;  // already initialized

    // Simple Sieve of Eratosthenes
    prime_small = (bool*)malloc((PRIME_SMALL_LIMIT + 1) * sizeof(bool));
    if (!prime_small) exit(1);
    for (size_t i = 0; i <= PRIME_SMALL_LIMIT; ++i) prime_small[i] = true;
    prime_small[0] = prime_small[1] = false;
    for (size_t i = 2; i * i <= PRIME_SMALL_LIMIT; ++i)
        if (prime_small[i])
            for (size_t j = i * i; j <= PRIME_SMALL_LIMIT; j += i)
                prime_small[j] = false;

    // Count primes to allocate array
    for (size_t i = 2; i <= PRIME_SMALL_LIMIT; ++i)
        if (prime_small[i]) prime_cnt++;

    primes = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    if (!primes) exit(1);
    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_SMALL_LIMIT; ++i)
        if (prime_small[i]) primes[idx++] = i;
}

// ---------------------------------------------------------------------
// Helper: test whether an even number k satisfies the condition
static bool good_even_k(seq_t k)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if ((seq_t)p > k / 2) break;          // no need to test larger p

        seq_t q = k - p;
        if (q < (seq_t)2) continue;

        bool q_is_prime;
        if ((uint64_t)q <= PRIME_SMALL_LIMIT && prime_small[(uint64_t)q])
            q_is_prime = true;
        else
            q_is_prime = isPrimeGeneric((seq_t)q);

        if (q_is_prime) {                     // minimal prime found
            unsigned __int128 cube = (unsigned __int128)p * p * p;
            return ((unsigned __int128)q) < cube;
        }
    }
    return false;    // should not occur (Goldbach conjecture holds up to tested range)
}

// ---------------------------------------------------------------------
// Helper: ensure sequence cache capacity
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < need) new_cap <<= 1;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

// ---------------------------------------------------------------------
// Public function as required
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  // undefined, but avoid misuse

    // One-time initialization
    if (!primes) init_primes();

    // Generate terms until we have at least n
    while (seq_size < (size_t)n) {
        // Find next even integer
        do {
            last_k += (seq_t)2;
        } while (!good_even_k(last_k));

        // Store the found term
        ensure_seq_capacity(seq_size + 1);
        seq_cache[seq_size++] = last_k;
    }

    return seq_cache[n - 1];
}
