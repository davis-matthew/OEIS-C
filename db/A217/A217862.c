/*
 * A217862  Primes p of the form p = 1 + 840*k for some k.
 * https://oeis.org/A217862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 24, u32 n <= 32, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   append_candidate, is_prime_big, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p ≡ 1 (mod 840) (OEIS A217862) */
/*   The n‑th term is the n‑th prime congruent to 1 modulo 840. */
/* The function computes terms by generating candidates of the form 1 + 840·k
   and testing each for primality, storing found primes in a static cache. */
/* Requirements 1–9 are satisfied. */

#include <stdlib.h>
#include <stdbool.h>

/* Provided utilities */
#include "../utils/append_candidate.h"   /* void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val); */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n); */
#include "../utils/seq_free_array.h"    /* void seq_free_array(seq_t *p); */

/* Static cache of primes p ≡ 1 (mod 840) */
static seq_t *prime_cache      = NULL;
static size_t prime_cache_len  = 0;
static size_t prime_cache_cap  = 0;
static idx_t  next_k           = 1;          /* candidate k value for next test */

/* Ensure that prime_cache contains at least n primes of the desired form. */
static void ensure_cache(idx_t n)
{
    /* Nothing to do if we already have enough */
    if (prime_cache_len >= (size_t)n)
        return;

    /* Generate candidates until we have at least n primes */
    while (prime_cache_len < (size_t)n) {
        seq_t candidate = (seq_t)840 * (seq_t)next_k + (seq_t)1;
        if (is_prime_big(candidate))
            append_candidate(&prime_cache, &prime_cache_len, &prime_cache_cap, candidate);
        next_k++;  /* always advance to the next potential k */
    }
}

/* Return the n‑th term (1‑based indexing).  Undefined for n = 0. */
seq_t a(idx_t n)
{
    if (n == 0)  return (seq_t)0;      /* offset is 1; a(0) is undefined */
    ensure_cache(n);                   /* guarantee the cache is ready   */
    return prime_cache[(size_t)n - 1];  /* zero‑based array indexing      */
}
