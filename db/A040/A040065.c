/*
 * A040065  Primes p such that x^3 = 13 has a solution mod p.
 * https://oeis.org/A040065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: sequence A040065
 *  a(n) = the n-th prime p such that the congruence x^3 ≡ 13 (mod p) has a solution.
 *  This is equivalent to: p = 2,3,13 or for other primes p
 *     if p ≡ 2 (mod 3) then true
 *     if p ≡ 1 (mod 3) then 13^{(p-1)/3} ≡ 1 (mod p)
 *
 *  The implementation generates primes incrementally and stores all primes
 *  that satisfy the condition in a cache.  Subsequent calls reuse the cache,
 *  regardless of the order of n.
 *
 *  It uses only integer arithmetic and the utility function mod_pow.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mod_pow.h"

/*--------------------- helper routines ---------------------*/

/* Test whether 13 is a cubic residue modulo p (p is a prime) */
static bool cubic_residue_of_13(uint64_t p)
{
    if (p == 2 || p == 3 || p == 13)            /* trivial cases */
        return true;
    if (p % 3 == 2)       /* p ≡ 2 (mod 3) → all residues are cubic */
        return true;
    /* p ≡ 1 (mod 3): check 13^{(p-1)/3} mod p == 1 */
    uint64_t exp = (p - 1) / 3;
    seq_t modp = (seq_t)p;
    seq_t val  = mod_pow(13, exp, modp);
    return val == (seq_t)1;
}

/* Append a value to a dynamic array.  The caller keeps track of used
 * length; this routine merely grows the underlying buffer if needed. */
static void grow_array(void **arr, size_t *cap, size_t new_len, size_t elem_size)
{
    if (new_len > *cap) {
        size_t new_cap = (*cap == 0) ? 8 : (*cap << 1);
        while (new_cap < new_len) new_cap <<= 1;
        void *tmp = realloc(*arr, new_cap * elem_size);
        if (!tmp) abort();          /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

/*--------------------- main sequence function ---------------------*/
extern seq_t a(idx_t n);

/* Global cache of primes satisfying the condition */
static seq_t *cache      = NULL;  /* a(i) values */
static size_t cache_used = 0;
static size_t cache_cap  = 0;

/* Global cache of all primes discovered so far (for primality tests) */
static uint64_t *primes      = NULL;
static size_t    primes_used = 0;
static size_t    primes_cap  = 0;

/* Candidate for next prime to test */
static uint64_t current_candidate = 2;

/* Primality test using already found primes (trial division) */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    /* Ensure we have enough known primes to test up to sqrt(x) */
    for (size_t i = 0; i < primes_used; ++i) {
        uint64_t p = primes[i];
        if (p * p > x) break;
        if (x % p == 0) return false;
    }
    return true;
}

/* Main sequence function */
extern seq_t a(idx_t n)
{
    /* Defensive: n must be >= 1 for validity of the sequence */
    if (n == 0) return (seq_t)0;

    /* Fill cache up to the requested index */
    while (cache_used < (size_t)n) {
        /* Find next prime */
        uint64_t p = current_candidate;
        while (!is_prime(p)) ++p;
        /* Append found prime to primes list */
        grow_array((void **)&primes, &primes_cap, primes_used + 1, sizeof(uint64_t));
        primes[primes_used++] = p;

        /* Test cubic residue condition */
        if (cubic_residue_of_13(p)) {
            grow_array((void **)&cache, &cache_cap, cache_used + 1, sizeof(seq_t));
            cache[cache_used++] = (seq_t)p;
        }
        /* Advance candidate for next iteration */
        current_candidate = p + 1;
    }

    /* Return requested term (sequence is 1‑based) */
    return cache[(size_t)n - 1];
}
