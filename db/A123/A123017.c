/*
 * A123017  Semiprimes k such that k+3 is also a semiprime.
 * https://oeis.org/A123017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n‑th number k such that k is a semiprime and k+3 is
 *            also a semiprime
 * Range:     u8 n <= 5, u16 n <= 920, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, floor_sqrt, is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple search with trial division against a prime list. */
/* It is suitable for seq_t up to 64‑bit as well as for the arbitrary‑precision build. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Utility headers provided by the build system */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t n) */
#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt) */
#include "../utils/seq_alloc.h"     /* seq_t *seq_alloc(size_t) */
#include "../utils/seq_realloc.h"   /* seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap) */

/* ---------- Internal state ------------------------------------------ */

/* Cache of computed terms */
static seq_t *cache = NULL;      /* array of computed a(n) values */
static size_t cache_len = 0;     /* number of terms stored in cache  */
static size_t cache_cap = 0;     /* current allocation capacity */

/* Last candidate number that was inspected.   */
static seq_t last_candidate = 2; /* next candidate to test */

/* Prime table used for factorisation tests */
static uint64_t *prime_tbl = NULL;  /* array of primes */
static size_t    prime_cnt = 0;     /* number of primes stored */
static idx_t     prime_limit = 0;   /* current maximum value in prime_tbl */

/* --------------------------------------------------------------------- */

/* Ensure that prime_tbl contains all primes up to at least `limit`.  */
static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Free the old table and allocate a new one. */
    if (prime_tbl != NULL) {
        free(prime_tbl);
        prime_tbl = NULL;
    }
    build_primes(limit, (idx_t **)&prime_tbl, &prime_cnt);
    prime_limit = limit;
}

/* Test whether a number is a semiprime: product of exactly two (not necessarily distinct) primes.  */
static bool
is_semiprime(seq_t n)
{
    /* Quick reject numbers below 4 (the smallest semiprime is 2*2 = 4). */
    if (n < 4)
        return false;

    /* If the number itself is prime, it cannot be semiprime. */
    if (is_prime_big(n))
        return false;

    /* Compute floor(sqrt(n)). */
    seq_t sq = floor_sqrt(n);

    /* Ensure prime table covers primes up to sqrt(n). */
    ensure_primes_upto((idx_t)sq);

    /* Trial division using the prime table. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];

        /* Stop if p > sqrt(n). */
        if ((seq_t)p > sq)
            break;

        if (n % (seq_t)p == 0) {
            seq_t q = n / (seq_t)p;
            /* Check that the co‑factor is also prime. */
            return (q >= 2) && is_prime_big(q);
        }
    }

    return false;   /* n is a product of more than two primes (or has a non‑prime factor) */
}

/* Grow the cache array to accommodate at least `min_cap` entries.  */
static void
grow_cache(size_t min_cap)
{
    if (cache_cap >= min_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;    /* double until sufficient */

    seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
    if (tmp == NULL)
        abort();          /* OOM */
    cache = tmp;
    cache_cap = new_cap;
}

/* --------------------------------------------------------------------- */

/* Public function required by the build system.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined, but keep safe */

    /* Lazy computation: generate terms until we have the nth.  */
    while (cache_len < n) {
        /* Test if the current candidate qualifies.  */
        if (is_semiprime(last_candidate) &&
            is_semiprime(last_candidate + 3)) {

            /* Store the new term.  */
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_candidate;
        }
        /* Advance to the next candidate. */
        ++last_candidate;
    }

    return cache[n - 1];
}
