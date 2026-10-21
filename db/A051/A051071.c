/*
 * A051071  Primes p such that x^4 = -2 has a solution mod p.
 * https://oeis.org/A051071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p such that the congruence x⁴ ≡ –2 (mod
 *            p) has a solution. It is computed by a simple sieve‑style search
 *            over primes and testing the quartic residue condition
 * Range:     u8 n <= 20, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mod_pow.h"

/* static storage for memoised results and prime list */
static seq_t   *results = NULL;
static size_t  results_len  = 0, results_cap  = 0;
static uint64_t *prime_list = NULL;
static size_t  prime_len    = 0, prime_cap    = 0;
static uint64_t next_candidate = 3;
static bool initialized = false;

/* ensure results array has at least min_cap slots */
static void grow_results(size_t min_cap) {
    if (results_cap < min_cap) {
        size_t new_cap = results_cap ? results_cap << 1 : 8;
        while (new_cap < min_cap) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(results, new_cap * sizeof(seq_t));
        results = tmp;
        results_cap = new_cap;
    }
}

/* ensure prime list has at least min_cap slots */
static void grow_primes(size_t min_cap) {
    if (prime_cap < min_cap) {
        size_t new_cap = prime_cap ? prime_cap << 1 : 8;
        while (new_cap < min_cap) new_cap <<= 1;
        uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
        prime_list = tmp;
        prime_cap   = new_cap;
    }
}

/* one‑time initialisation: store 2 as first result and first prime */
static void init(void) {
    if (initialized) return;
    grow_results(1);
    results[0] = (seq_t)2;
    results_len = 1;

    grow_primes(1);
    prime_list[0] = 2;
    prime_len = 1;

    next_candidate = 3;
    initialized = true;
}

/* test whether a prime p satisfies the quartic residue condition */
static bool good_prime(uint64_t p) {
    if (p == 2) return true;  /* 2 is always included */
    seq_t base = (seq_t)(p - 2);                 /* –2 modulo p */
    seq_t leg  = mod_pow(base, (p - 1) / 2, (seq_t)p);
    if (leg != (seq_t)1) return false;           /* not a quadratic residue */
    if (p % 4 == 3) return true;                 /* all quadratic residues are 4th powers */
    seq_t t = mod_pow(base, (p - 1) / 4, (seq_t)p);
    return t == (seq_t)1;                       /* fourth‑power condition for p≡1 (mod 4) */
}

/* build results up to the n‑th term */
static void ensure_results(idx_t n) {
    init();
    while (results_len < n) {
        uint64_t p = next_candidate;
        /* trial division using stored primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len && prime_list[i] <= p / prime_list[i]; ++i) {
            if (p % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            grow_primes(prime_len + 1);
            prime_list[prime_len++] = p;
            if (good_prime(p)) {
                grow_results(results_len + 1);
                results[results_len++] = (seq_t)p;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* public sequence function */
seq_t a(idx_t n) {
    ensure_results(n);
    return results[n - 1];
}
