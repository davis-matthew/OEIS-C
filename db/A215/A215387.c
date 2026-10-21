/*
 * A215387  Primes congruent to {2, 3, 6} mod 13.
 * https://oeis.org/A215387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   expand_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Theorem:  a(n) is the n‑th prime congruent to 2, 3, or 6 modulo 13.
   This implementation builds the set incrementally: it extends a table
   of all primes found so far to test new candidates by trial division
   (which is sufficient for n ≤ 10 000).  The target primes are stored
   in a separate cache. The routine is re‑entrant and works for any n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/expand_cache.h"

static seq_t *prime_cache   = NULL;   /* all primes discovered, needed for primality checks */
static idx_t prime_len      = 0;
static size_t prime_cap     = 0;

static seq_t *res_cache     = NULL;   /* primes ≡ 2,3,6 (mod 13) */
static idx_t res_len        = 0;
static size_t res_cap       = 0;

static idx_t next_candidate = 2;      /* next integer to test for primality */

/* Ensure that res_cache contains at least needed primes. */
static void
ensure_cache(idx_t needed)
{
    if (res_len >= needed) return;

    /* Ensure prime_cache is large enough to hold the primes discovered so far. */
    if (prime_cap == 0) {
        expand_cache(&prime_cache, &prime_cap, 128);
    }

    /* Extend res_cache as well. */
    if (res_cap == 0) {
        expand_cache(&res_cache, &res_cap, 128);
    }

    while (res_len < needed) {
        bool isprime = true;
        for (idx_t i = 0; i < prime_len; ++i) {
            seq_t p = prime_cache[i];
            if ((seq_t)p * (seq_t)p > (seq_t)next_candidate) break;
            if (next_candidate % p == 0) { isprime = false; break; }
        }

        if (isprime) {
            /* store prime for future trial division */
            if (prime_len == prime_cap) {
                expand_cache(&prime_cache, &prime_cap, prime_cap * 2);
            }
            prime_cache[prime_len++] = next_candidate;

            /* if it satisfies the residue condition, store it in result cache */
            if (next_candidate % 13 == 2
                || next_candidate % 13 == 3
                || next_candidate % 13 == 6) {
                if (res_len == res_cap) {
                    expand_cache(&res_cache, &res_cap, res_cap * 2);
                }
                res_cache[res_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;
    ensure_cache(n);
    return res_cache[n - 1];
}
