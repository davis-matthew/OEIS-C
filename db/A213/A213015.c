/*
 * A213015  Numbers n such that the sum of prime factors of n (counted with
 *          multiplicity) is 2 times a prime.
 * https://oeis.org/A213015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A213015: Numbers n such that the sum of prime factors of n (counted with multiplicity) equals 2 × a prime.   
   This implementation builds a linear sieve for smallest prime factors, computes the sum of prime factors (sopfr) for each integer, and stores all n that satisfy the condition. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"   /* provides bool is_prime_big(seq_t x) */

static seq_t *cache      = NULL;    /* cached sequence values */
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

static uint32_t *spf       = NULL;  /* smallest prime factor array */
static size_t     spf_cap  = 0;
static seq_t    *sopfr     = NULL;  /* sum of prime factors with multiplicity */
static size_t    sopfr_cap = 0;

static uint32_t *primes    = NULL;  /* list of primes found by linear sieve */
static size_t    primes_len   = 0;
static size_t    primes_cap   = 0;

static uint64_t processed = 1;      /* highest integer that has been processed */


/* Append a new sequence element to the cache, extending storage if necessary. */
static void append_cache(seq_t val)
{
    if (cache_len == cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 8;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = val;
}

/* Ensure the primes array has enough capacity. */
static void ensure_primes_cap(size_t needed)
{
    if (primes_cap < needed) {
        primes_cap = (primes_cap ? primes_cap * 2 : 8);
        if (primes_cap < needed) primes_cap = needed;
        primes = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
    }
}

/* Extend the sieve (and generated sequence) up to at least new_limit. */
static void extend_sieve(uint64_t new_limit)
{
    if (new_limit <= spf_cap) return;

    /* Resize spf array */
    if (spf_cap < new_limit + 1) {
        spf_cap = new_limit + 1;
        spf = (uint32_t *)realloc(spf, spf_cap * sizeof(uint32_t));
    }

    /* Linear sieve to fill spf up to new_limit. */
    for (uint64_t i = 2; i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            ensure_primes_cap(primes_len + 1);
            primes[primes_len++] = (uint32_t)i;
        }
        for (size_t pi = 0; pi < primes_len; ++pi) {
            uint32_t p = primes[pi];
            uint64_t ip = i * (uint64_t)p;
            if (ip > new_limit || p > spf[i]) break;
            spf[ip] = p;
        }
    }

    /* Resize sopfr array */
    if (sopfr_cap < new_limit + 1) {
        sopfr_cap = new_limit + 1;
        sopfr = (seq_t *)realloc(sopfr, sopfr_cap * sizeof(seq_t));
    }
    sopfr[0] = sopfr[1] = 0;

    /* Compute sopfr for all numbers up to new_limit. */
    for (uint64_t i = 2; i <= new_limit; ++i) {
        uint32_t f = spf[i];
        sopfr[i] = (seq_t)f + sopfr[i / f];
    }

    /* Scan the newly computed numbers for qualifying n. */
    for (uint64_t i = processed + 1; i <= new_limit; ++i) {
        seq_t sum = sopfr[i];
        if ((sum & (seq_t)1) == 0) {                /* even sum */
            seq_t half = sum / (seq_t)2;
            if (is_prime_big(half)) {
                append_cache((seq_t)i);
            }
        }
    }
    processed = new_limit;
}

/* Public sequence function as required by the framework. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out-of-range fallback */

    /* Grow the sieve and cache until we have at least n terms. */
    while (cache_len < (size_t)n) {
        uint64_t new_limit = spf_cap ? spf_cap * 2 : 16;
        if (new_limit < 16) new_limit = 16;
        extend_sieve(new_limit);
    }
    return cache[n - 1];
}
