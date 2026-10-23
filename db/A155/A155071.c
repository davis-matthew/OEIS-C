/*
 * A155071  Primes with two odd digits.
 * https://oeis.org/A155071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_two_odd_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155071: primes with two odd digits.  The nth term is the
   nth prime whose decimal representation contains exactly two odd
   digits (1,3,5,7,9).  The function builds the sequence incrementally
   using trial‑division prime generation and an odd‑digit test. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/has_two_odd_digits.h"

/* --- cache for the sequence --------------------------------------- */
static seq_t *seq_cache      = NULL;
static size_t cache_len      = 0;
static size_t cache_cap      = 0;

/* --- dynamic array of primes found during generation --------------- */
static uint64_t *prime_vec   = NULL;
static size_t   prime_len    = 0;
static size_t   prime_cap    = 0;

/* --- grow the sequence cache -------------------------------------- */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* --- grow the prime vector ---------------------------------------- */
static void ensure_prime_capacity(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 64;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *tmp = (uint64_t *)realloc(prime_vec, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_vec = tmp;
    prime_cap = new_cap;
}

/* --- generate the next prime in increasing order using trial division */
static uint64_t next_prime(void)
{
    static uint64_t candidate = 2;
    for (;;) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_len; ++i) {
            uint64_t p = prime_vec[i];
            if (p > candidate / p) break;            /* p*p > candidate */
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            /* store the prime */
            ensure_prime_capacity(prime_len + 1);
            prime_vec[prime_len++] = candidate;
            uint64_t result = candidate;
            candidate = (candidate == 2) ? 3 : candidate + 2;  /* skip evens */
            return result;
        } else {
            /* move to the next candidate (odd numbers only, except 2) */
            candidate = (candidate <= 2) ? 3 : candidate + 2;
        }
    }
}

/* --- main sequence function --------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safe return for undefined offset */

    while (cache_len < (size_t)n) {
        uint64_t p64 = next_prime();
        seq_t p = (seq_t)p64;
        if (has_two_odd_digits(p)) {
            ensure_cache_capacity(cache_len + 1);
            seq_cache[cache_len++] = p;
        }
    }
    return seq_cache[(size_t)n - 1];
}
