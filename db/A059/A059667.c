/*
 * A059667  Primes p such that x^49 = 2 has no solution mod p, but x^7 = 2 has
 *          a solution mod p.
 * https://oeis.org/A059667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 22, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: The nth prime p such that x^49 = 2 has no solution mod p,
 * while x^7 = 2 has a solution.  Equivalently p ≡ 1 (mod 49),
 * 2^((p‑1)/7) ≡ 1 (mod p), and 2^((p‑1)/49) ≠ 1 (mod p).
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/mod_pow.h"   /* mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/is_prime.h"  /* is_prime(idx_t x) */

static seq_t *cache      = NULL; /* array of found primes */
static size_t cache_cap  = 0;    /* current capacity of cache */
static size_t cache_len  = 0;    /* number of primes stored */

static idx_t next_candidate = 50; /* first candidate ≡ 1 (mod 49) */

/* Ensure that at least n terms have been computed and stored in cache. */
static void ensure_seq(idx_t n)
{
    if (cache_len >= (size_t)n) return;          /* already computed */

    /* Grow cache if necessary. */
    if (cache_cap < cache_len + 1) {
        size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);     /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Search for further primes meeting the condition. */
    while (cache_len < (size_t)n) {
        /* Check primality first. */
        if (is_prime(next_candidate)) {
            /* p must satisfy 49 | (p-1). */
            if (next_candidate % 49 == 1) {
                seq_t p = (seq_t)next_candidate;
                uint64_t exp7  = (next_candidate - 1) / 7;   /* (p‑1)/7   */
                uint64_t exp49 = exp7 / 7;                   /* (p‑1)/49  */

                /* 2^((p‑1)/7) must be 1 mod p and 2^((p‑1)/49) must not be 1. */
                if (mod_pow((seq_t)2, exp7, p) == 1 &&
                    mod_pow((seq_t)2, exp49, p) != 1) {
                    cache[cache_len++] = p;
                }
            }
        }
        next_candidate += 49;  /* next number ≡ 1 (mod 49) */
    }
}

/* Public accessor. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but return 0 */
    ensure_seq(n);
    return cache[n - 1];
}
