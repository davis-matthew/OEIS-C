/*
 * A107137  Primes of the form 2x^2 + 15y^2.
 * https://oeis.org/A107137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the primes of the form 2*x^2 + 15*y^2.
   The prime must satisfy p ≡ {2, 17, 23, 47, 113} (mod 120). */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Buffer to test primality during sieving. */
static uint8_t residue_ok[120] = {0};
static const int residues[5] = {2, 17, 23, 47, 113};

/* Default bound to start with; the sieve doubles each round. */
#define INITIAL_LIMIT 200000ULL

/* Cache of primes that satisfy the condition. */
static uint64_t *prime_cache = NULL;
static idx_t prime_cache_size   = 0;
static idx_t prime_cache_capacity = 0;

/* Current upper bound of the sieve. */
static uint64_t sieve_limit = 0;

/* ------------------------------------------------------------------ */
/* Initialise the table of allowed residues. */
static void init_residues(void)
{
    memset(residue_ok, 0, sizeof(residue_ok));
    for (int i = 0; i < 5; ++i)
        residue_ok[residues[i]] = 1;
}

/* ------------------------------------------------------------------ */
/* Append a prime to the cache, resizing the array if necessary. */
static void add_prime(uint64_t p)
{
    if (prime_cache_size == prime_cache_capacity) {
        idx_t new_cap = prime_cache_capacity == 0 ? 128 : prime_cache_capacity * 2;
        uint64_t *tmp = (uint64_t *)realloc(prime_cache, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        prime_cache = tmp;
        prime_cache_capacity = new_cap;
    }
    prime_cache[prime_cache_size++] = p;
}

/* ------------------------------------------------------------------ */
/* Rebuild the sieve up to 'limit' and repopulate the cache. */
static void recompute(uint64_t limit)
{
    /* Allocate the sieve array. */
    uint8_t *is_prime = (uint8_t *)malloc((size_t)(limit + 1));
    if (!is_prime) abort();
    memset(is_prime, 1, (size_t)(limit + 1));
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;

    /* Sieve of Eratosthenes. */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Rebuild the cache of qualifying primes. */
    prime_cache_size = 0;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (is_prime[p] && residue_ok[p % 120])
            add_prime(p);
    }

    sieve_limit = limit;
    free(is_prime);
}

/* ------------------------------------------------------------------ */
/* Public sequence function. */
seq_t a(idx_t n)
{
    static int residues_inited = 0;
    if (!residues_inited) {
        init_residues();
        residues_inited = 1;
    }

    if (n == 0) return (seq_t)0;   /* offset is 1, but guard against zero. */

    /* Ensure the cache contains at least n primes. */
    while (prime_cache_size < n) {
        uint64_t new_limit = sieve_limit == 0 ? INITIAL_LIMIT : sieve_limit * 2;
        recompute(new_limit);
    }

    return (seq_t)prime_cache[n - 1];
}
