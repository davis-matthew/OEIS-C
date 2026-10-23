/*
 * A045318  Primes p such that x^8 = 3 has no solution mod p.
 * https://oeis.org/A045318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: primes p such that x^8 = 3 has no solution mod p
   (OEIS A045318) */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/gcd.h"          /* gcd(seq_t, seq_t) */
#include "../utils/mod_pow.h"      /* mod_pow(seq_t, uint64_t, seq_t) */
#include "../utils/next_prime.h"   /* next_prime(uint64_t) */

/* 64‑bit greatest common divisor helper */
static uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Cache of qualifying primes (stored as 64‑bit values). */
static uint64_t *prime_cache = NULL;
static size_t cache_size  = 0;   /* number of entries filled */
static size_t cache_cap   = 0;   /* allocated capacity */

/* Ensure that prime_cache contains the first n qualifying primes. */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return;  /* already have enough */

    /* Allocate or grow cache storage. */
    if (cache_cap == 0) {
        cache_cap   = 64;
        prime_cache = (uint64_t *)malloc(cache_cap * sizeof(uint64_t));
    }

    uint64_t curr;
    if (cache_size == 0)
        curr = 5;                            /* first prime to test */
    else
        curr = next_prime(prime_cache[cache_size - 1] + 1);

    while (cache_size < (size_t)n) {
        /* Compute d = gcd(8, p-1) and exp = (p-1)/d. */
        uint64_t d   = gcd64(8ULL, curr - 1);
        uint64_t exp = (curr - 1) / d;

        /* x^8≡3 has no solution ⇔ 3^exp ≠ 1 (mod p). */
        if (mod_pow((seq_t)3, exp, curr) != (seq_t)1) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                prime_cache = (uint64_t *)realloc(prime_cache,
                                                  cache_cap * sizeof(uint64_t));
            }
            prime_cache[cache_size++] = curr;
        }

        curr = next_prime(curr + 1);
    }
}

/* Return the n‑th member of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return (seq_t)prime_cache[n - 1];
}
