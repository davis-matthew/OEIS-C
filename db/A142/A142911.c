/*
 * A142911  Primes congruent to 40 mod 63.
 * https://oeis.org/A142911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Primes congruent to 40 (mod 63)
   a(1) = 103, a(2) = 229, a(3) = 607, …   (OEIS A142911) */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t n); */

/* Cache for the found primes ≡ 40 (mod 63) */
static seq_t *prime_cache  = NULL;   /* array of cached primes          */
static size_t prime_cnt    = 0;      /* number of cached primes          */
static size_t prime_cap    = 0;      /* current allocated capacity       */
static uint64_t last_checked = 0;    /* last candidate examined          */

/* Ensure the cache can hold at least 'needed' primes. */
static void grow_cache(size_t needed)
{
    if (prime_cap >= needed)
        return;
    size_t new_cap = (prime_cap == 0) ? 16 : prime_cap * 2;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp) {              /* allocation failed – abort */
        exit(EXIT_FAILURE);
    }
    prime_cache = tmp;
    prime_cap   = new_cap;
}

/* Make sure the cache contains at least 'n' primes. */
static void ensure_primes(idx_t n)
{
    if (prime_cnt >= (size_t)n)
        return;

    grow_cache((size_t)n);

    /* Find the first integer ≥ last_checked+1 that is ≡ 40 (mod 63). */
    uint64_t cand = last_checked + 1;
    uint64_t rem  = cand % 63;
    uint64_t delta = (40 + 63 - rem) % 63;
    cand += delta;

    while (prime_cnt < (size_t)n) {
        if (is_prime(cand)) {
            prime_cache[prime_cnt++] = (seq_t)cand;
        }
        cand += 63;                /* stay in the arithmetic progression */
    }

    /* Update for the next call.  The last tested candidate is
       one step behind the final 'cand' value. */
    last_checked = cand - 63;
}

/* Return the n-th prime ≡ 40 (mod 63); n is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0)      /* undefined for n<1 – behaviour irrelevant */
        return 0;

    ensure_primes(n);
    return prime_cache[n - 1];
}
