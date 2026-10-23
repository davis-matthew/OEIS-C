/*
 * A040998  Primes p such that x^6 = 5 has a solution mod p.
 * https://oeis.org/A040998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_cap_seqt, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes and test the congruence
 * 5^((p-1)/gcd(6,p-1)) ≡ 1 (mod p) to obtain primes p for A040998. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"
#include "../utils/ensure_cap_seqt.h"

/* Ensure a dynamic array has the required capacity. */
static void ensure_cap_uint64(size_t needed, size_t *cap, uint64_t **ptr)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
    if (new_cap < needed) new_cap = needed;
    uint64_t *new_ptr = (uint64_t*)realloc(*ptr, new_cap * sizeof(uint64_t));
    *ptr = new_ptr;
    *cap = new_cap;
}

/* Global state for primes and qualifying primes. */
static uint64_t *global_primes = NULL;      /* All primes seen so far. */
static size_t primes_cnt = 0, primes_cap = 0;

/* Cache of primes satisfying the property. */
static seq_t *global_cache = NULL;
static size_t cache_cnt = 0, cache_cap = 0;

/* The last integer tested for primality. */
static uint64_t last_candidate = 1;

/* Trial‑division primality test using the stored primes. */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = global_primes[i];
        __uint128_t prod = (__uint128_t)p * p;
        if (prod > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Test whether 5 is a sixth‑power residue modulo p. */
static bool good_prime(uint64_t p)
{
    if (p == 2 || p == 5) return true;  /* trivial cases */

    /* Compute d = gcd(6, p-1). */
    uint64_t rem = (p - 1) % 6;
    uint64_t d = (rem == 0) ? 6 : (rem == 3) ? 3 : 2;

    idx_t exp = (idx_t)((p - 1) / d);
    seq_t val = mod_pow((seq_t)5, exp, (seq_t)p);
    return val == (seq_t)1;
}

/* Fill the cache until it contains at least n qualifying primes. */
static void ensure_n_terms(idx_t n)
{
    while (cache_cnt < (size_t)n) {
        /* Find next prime candidate. */
        do { ++last_candidate; } while (!is_prime_uint64(last_candidate));

        /* Store the new prime for future primality checks. */
        ensure_cap_uint64(primes_cnt + 1, &primes_cap, &global_primes);
        global_primes[primes_cnt++] = last_candidate;

        /* If it satisfies the property, cache it. */
        if (good_prime(last_candidate)) {
            ensure_cap_seqt(cache_cnt + 1, &cache_cap, &global_cache);
            global_cache[cache_cnt++] = (seq_t)last_candidate;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1; undefined for 0 */
    ensure_n_terms(n);
    return global_cache[(size_t)n - 1];
}
