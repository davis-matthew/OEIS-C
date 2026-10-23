/*
 * A019390  Primes with primitive root 67.
 * https://oeis.org/A019390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A019390: primes p such that the multiplicative order of 67 mod p is p-1.
   The routine generates primes incrementally and tests each candidate for
   the primitive‑root property, caching results to handle arbitrary call
   order.  The test must exclude p = 67 itself.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mod_pow.h"

/* Prime list used for trial division and factorisation of p‑1. */
static idx_t *prime_list  = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

/* Cache of primes satisfying the property – indexed from 0. */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Ensure enough space in prime_list. */
static void grow_primes(size_t need)
{
    if (need <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    idx_t *newp = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (!newp) exit(1);
    if (prime_list) {
        memcpy(newp, prime_list, prime_count * sizeof(idx_t));
        free(prime_list);
    }
    prime_list = newp;
    prime_cap  = new_cap;
}

/* Ensure enough space in the result cache. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) exit(1);
    if (cache) {
        memcpy(newp, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = newp;
    cache_cap = new_cap;
}

/* Test whether prime p satisfies ord_p(67) = p-1. */
static bool has_primitive_root_67(idx_t p)
{
    /* p = 2 is a valid primitive‑root prime. */
    if (p == 2) return true;
    /* p = 67 is excluded by definition (67 mod 67 is 0). */
    if (p == 67) return false;

    uint64_t phi = p - 1;
    uint64_t tmp = phi;
    idx_t pf[64];
    size_t pf_cnt = 0;

    /* Factor phi using the known primes. */
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= tmp; ++i) {
        idx_t q = prime_list[i];
        if (tmp % q == 0) {
            pf[pf_cnt++] = q;
            do { tmp /= q; } while (tmp % q == 0);
        }
    }
    if (tmp > 1) pf[pf_cnt++] = tmp;

    /* Check the defining property for each prime factor. */
    for (size_t i = 0; i < pf_cnt; ++i) {
        idx_t q = pf[i];
        if (mod_pow((seq_t)67, (uint64_t)(phi / q), (seq_t)p) == (seq_t)1)
            return false;
    }
    return true;
}

/* Produce primes and cache until the requested index n is reached. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n) return;

    /* Initialise with prime 2 if necessary. */
    if (prime_count == 0) {
        grow_primes(1);
        prime_list[prime_count++] = 2;
        grow_cache(1);
        cache[cache_size++] = (seq_t)2;   /* 2 satisfies the property */
    }

    static idx_t next_candidate = 3;    /* start scanning from 3 */

    while (cache_size < (size_t)n) {
        idx_t cand = next_candidate;
        next_candidate += 2;

        /* Trial‑division primality test. */
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t q = prime_list[i];
            if (q * q > cand) break;
            if (cand % q == 0) { is_prime = false; break; }
        }
        if (!is_prime) continue;

        /* Add the new prime to the list. */
        grow_primes(prime_count + 1);
        prime_list[prime_count++] = cand;

        /* Check the primitive‑root condition. */
        if (has_primitive_root_67(cand)) {
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)cand;
        }
    }
}

/* Sequence API required by the benchmark harness. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    ensure_cache(n);
    return cache[n - 1];
}
