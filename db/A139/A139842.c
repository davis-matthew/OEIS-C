/*
 * A139842  Primes of the form 3x^2 + 34y^2.
 * https://oeis.org/A139842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 827, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A139842: primes of the form 3*x^2 + 34*y^2.
   The admissible primes are exactly those congruent to
   one of the residues
   3,37,61,91,109,133,139,163,181,211,235,277,283,301,379,397,403 (mod 408). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"     /* is_prime(idx_t) : primality test */
#include "../utils/seq_realloc.h"  /* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

/* The allowed residue classes modulo 408. */
static const unsigned long long ALLOWED_RESIDUES[] = {
    3ULL, 37ULL, 61ULL, 91ULL, 109ULL, 133ULL, 139ULL,
    163ULL, 181ULL, 211ULL, 235ULL, 277ULL, 283ULL,
    301ULL, 379ULL, 397ULL, 403ULL
};
static const size_t NUM_ALLOWED = sizeof(ALLOWED_RESIDUES) / sizeof(ALLOWED_RESIDUES[0]);

/* Cache of found primes. */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_len  = 0;
static idx_t next_candidate = 3;  /* next odd number to be tested */

/* Double the capacity of the cache when full. */
static void grow_cache(void)
{
    size_t new_cap = (cache_cap == 0) ? 128 : cache_cap * 2;
    seq_t *newp = seq_realloc(cache, cache_cap, new_cap);
    if (!newp) abort();           /* out of memory */
    cache = newp;
    cache_cap = new_cap;
}

/* Check whether p has a residue allowed for the form 3x^2+34y^2. */
static bool prime_of_form(idx_t p)
{
    unsigned long long r = p % 408ULL;
    for (size_t i = 0; i < NUM_ALLOWED; ++i)
        if (r == ALLOWED_RESIDUES[i]) return true;
    return false;
}

/* Ensure that cache holds at least n primes of the required form. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        if (is_prime(next_candidate)) {
            if (prime_of_form(next_candidate)) {
                if (cache_len == cache_cap) grow_cache();
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        next_candidate += 2;   /* skip even numbers */
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset 1; a(1) is the first prime */
    ensure_cache(n);
    return cache[n - 1];
}
