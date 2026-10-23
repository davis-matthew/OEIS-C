/*
 * A105890  Primes for which -18 is a primitive root.
 * https://oeis.org/A105890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A105890: a(n) = the n-th prime p such that -18 is a primitive
   root modulo p. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mod_pow.h"

static uint64_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_len = 0;

static uint64_t *found = NULL;
static size_t found_cap = 0;
static size_t found_len = 0;

static uint64_t last_candidate = 3;  /* primes 2 and 3 are pre‑seeded */

static void ensure_prime_cap(size_t min_cap) {
    if (prime_cap >= min_cap) return;
    size_t new_cap = prime_cap ? prime_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_list = tmp;
    prime_cap = new_cap;
}

static void ensure_found_cap(size_t min_cap) {
    if (found_cap >= min_cap) return;
    size_t new_cap = found_cap ? found_cap << 1 : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(found, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    found = tmp;
    found_cap = new_cap;
}

static void push_prime(uint64_t v) {
    ensure_prime_cap(prime_len + 1);
    prime_list[prime_len++] = v;
}

static void push_found(uint64_t v) {
    ensure_found_cap(found_len + 1);
    found[found_len++] = v;
}

static bool is_primitive_root(uint64_t p) {
    if (p <= 3) return false;  /* trivial cases */
    uint64_t phi = p - 1;
    uint64_t base = (p - (18 % p)) % p;  /* (-18) mod p */
    uint64_t t = phi;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t q = prime_list[i];
        if (q > t / q) break;           /* q*q > t */
        if (t % q == 0) {
            if (mod_pow(base, phi / q, p) == 1) return false;
            while (t % q == 0) t /= q;
        }
    }
    if (t > 1) {  /* remaining prime factor */
        if (mod_pow(base, phi / t, p) == 1) return false;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined, but return 0 safely */
    /* Lazy initialization of data structures. */
    if (!prime_list) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        if (!prime_list) abort();
        found_cap = 16;
        found = (uint64_t *)malloc(found_cap * sizeof(uint64_t));
        if (!found) abort();
        /* seed the prime list with the first primes */
        push_prime(2);
        push_prime(3);
    }

    size_t target = (size_t)n;
    while (found_len < target) {
        uint64_t cand = ++last_candidate;
        if ((cand & 1) == 0) continue;          /* skip even numbers >2 */
        /* Test primality using the current prime list. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_list[i];
            if (p > cand / p) break;            /* p*p > cand */
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (!is_prime) continue;
        push_prime(cand);
        if (cand >= 5 && is_primitive_root(cand)) {
            push_found(cand);
        }
    }

    return (seq_t)found[n - 1];
}
