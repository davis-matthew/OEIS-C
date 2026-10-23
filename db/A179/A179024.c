/*
 * A179024  Primes whose only prime digits are 2's.
 * https://oeis.org/A179024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 412, u32 n <= 353, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime whose decimal digits contain only the prime
   digit 2 (and at least one 2), as in OEIS A179024.
   The implementation searches for primes incrementally and memoises
   the results, so the function works for arbitrary order of calls. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Cache of discovered primes */
static seq_t *prime_cache   = NULL;  /* array of found primes          */
static size_t  cache_cap    = 0;     /* allocated array capacity       */
static size_t  cache_len    = 0;     /* number of primes stored        */
static idx_t   last_checked = 0;     /* largest integer examined so far */

/* ------------------------------------------------------------------ */
/* Helper: check that a decimal integer consists only of the prime
   digit 2 (digits 3,5,7 are forbidden) and contains at least one 2. */
static bool allowed_digits(idx_t x)
{
    bool has_two = false;
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d == 3 || d == 5 || d == 7) return false;
        if (d == 2) has_two = true;
        x /= 10;
    }
    return has_two;
}

/* ------------------------------------------------------------------ */
/* Append a new prime to the cache, expanding the storage if needed. */
static void append_prime(seq_t p)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        prime_cache = (seq_t *)realloc(prime_cache, cache_cap * sizeof(seq_t));
        if (!prime_cache) abort();
    }
    prime_cache[cache_len++] = p;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n primes. */
    while (cache_len < n) {
        /* Search for the next candidate integer. */
        for (idx_t cand = last_checked + 1; ; ++cand) {
            if (!allowed_digits(cand)) continue;

            /* Convert to seq_t for prime test. */
            seq_t cand_seq = (seq_t)cand;
            if (is_prime_big(cand_seq)) {
                append_prime(cand_seq);
                last_checked = cand;
                break;   /* prime found; advance to next missing slot */
            }
        }
    }
    return prime_cache[n - 1];
}
