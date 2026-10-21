/*
 * A212377  Primes congruent to 1 mod 53.
 * https://oeis.org/A212377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 123, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow, mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = the n-th prime p such that p ≡ 1 (mod 53).
 * This function generates primes congruent to 1 modulo 53 by 
 * incremental trial of numbers 107 + 53*k, using an efficient
 * deterministic Miller–Rabin test for 64‑bit integers.
 * The results are stored in a static cache to allow arbitrary
 * ordering of calls and to keep each call well below the time limit.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul64.h"
#include "../utils/mod_pow.h"

/* Deterministic Miller–Rabin for 64‑bit integers */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;

    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }

    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;

        seq_t x_seq = mod_pow((seq_t)a, d, (seq_t)n);
        uint64_t x = (uint64_t)x_seq;
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* Cache for the sequence values */
static seq_t *prime_cache = NULL;
static size_t prime_cache_size = 0;
static size_t prime_cache_cap   = 0;
static uint64_t next_candidate  = 107;      /* first number ≡ 1 (mod 53) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe */

    /* Grow cache until we have at least n entries */
    while (prime_cache_size < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 53;   /* next number that is ≡ 1 (mod 53) */

        if (isPrime64(cand)) {
            if (prime_cache_size == prime_cache_cap) {
                prime_cache_cap = prime_cache_cap ? prime_cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(prime_cache,
                                              prime_cache_cap * sizeof(seq_t));
                if (!tmp) abort();          /* allocation failure */
                prime_cache = tmp;
            }
            prime_cache[prime_cache_size++] = (seq_t)cand;
        }
    }
    return prime_cache[n - 1];
}
