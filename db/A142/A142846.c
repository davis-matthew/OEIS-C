/*
 * A142846  Primes congruent to 48 mod 61.
 * https://oeis.org/A142846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p with p ≡ 48 (mod 61)
 * Range:     u8 n <= 1, u16 n <= 104, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A142846 – primes congruent to 48 modulo 61 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* ------------------------------------------------------------------ */
/*  Cache of the computed primes                                       */
/* ------------------------------------------------------------------ */
static seq_t *prime_cache = NULL;   /* array of cached terms          */
static size_t prime_cache_size = 0; /* number of terms currently stored */
static size_t prime_cache_cap  = 0; /* current allocated capacity      */
static uint64_t prime_k         = 0; /* last k used in 61*k+48 form   */

/* ------------------------------------------------------------------ */
/*  Ensure the prime cache can hold at least `need` terms             */
/* ------------------------------------------------------------------ */
static void grow_cache(size_t need)
{
    if (need <= prime_cache_cap)
        return;
    size_t new_cap = prime_cache_cap ? prime_cache_cap * 2 : 4;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!tmp)                  /* out of memory */
        abort();
    prime_cache   = tmp;
    prime_cache_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/*  Compute the n-th prime that is 48 (mod 61)                         */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;              /* undefined for n < 1 */

    /* If the cache already holds the term, return it */
    if (prime_cache_size >= (size_t)n)
        return prime_cache[n - 1];

    /* ------------------------------------------------------------------
       Initialise the cache if it is empty.  The first prime of the form
       61*k+48 is 61*1+48 = 109, and it is indeed prime.
       ------------------------------------------------------------------ */
    if (prime_cache_size == 0) {
        prime_k = 1;           /* start with k = 1 (odd) */
        uint64_t cand = 61ULL * prime_k + 48ULL;
        /* Verify the first candidate; if it were not prime, keep searching
           odd k until a prime is found. This guarantees the first term
           is correct even if the initial guess were wrong. */
        while (!is_prime((idx_t)cand)) {
            prime_k += 2;      /* keep k odd; candidate stays odd */
            cand = 61ULL * prime_k + 48ULL;
        }
        grow_cache(1);
        prime_cache[0] = (seq_t)cand;
        prime_cache_size = 1;
    }

    /* ------------------------------------------------------------------
       Generate further primes until we reach the desired index.          */
    /* ------------------------------------------------------------------ */
    while (prime_cache_size < (size_t)n) {
        /* move to the next odd k */
        prime_k += 2;
        uint64_t cand = 61ULL * prime_k + 48ULL;

        while (!is_prime((idx_t)cand)) {
            prime_k += 2;
            cand = 61ULL * prime_k + 48ULL;
        }

        /* store the found prime */
        grow_cache(prime_cache_size + 1);
        prime_cache[prime_cache_size] = (seq_t)cand;
        ++prime_cache_size;
    }

    return prime_cache[n - 1];
}
