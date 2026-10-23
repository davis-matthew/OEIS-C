/*
 * A045421  Primes congruent to {0, 3, 4} mod 7.
 * https://oeis.org/A045421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A045421:
   a(n) = the n-th prime p such that p ≡ 0, 3, 4 (mod 7) */
/* The function uses incremental primality testing with memoisation */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the sequence values (cache) */
static seq_t *cache   = NULL; /* array of qualifying primes */
static size_t cache_len = 0;  /* number of entries stored */
static size_t cache_cap = 0;  /* allocated capacity */

/* static storage for prime numbers found so far (used for primality test) */
static uint64_t *primes   = NULL; /* array of small primes */
static size_t primes_len   = 0;    /* number of primes stored */
static size_t primes_cap   = 0;    /* allocated capacity */

/* next integer to test for primality (the sieve is incremental) */
static uint64_t next_candidate = 2;

/* ------------------------------------------------------------------ */
/* helpers to grow the dynamic arrays                                   */
/* ------------------------------------------------------------------ */
static void cache_push(seq_t val)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

static void primes_push(uint64_t p)
{
    if (primes_len >= primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_len++] = p;
}

/* ------------------------------------------------------------------ */
/* primality test using the current list of primes                     */
/* ------------------------------------------------------------------ */
static bool is_prime(uint64_t x)
{
    if (x < 2) return false;
    for (size_t i = 0; i < primes_len; ++i) {
        uint64_t p = primes[i];
        if (p > x / p) break;            /* p*p > x */
        if (x % p == 0) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* main sequence function                                               */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n < 1 */

    /* return cached value if already known */
    if ((size_t)n <= cache_len) return cache[n - 1];

    /* generate qualifying primes until we have n of them */
    while (cache_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* advance candidate to next integer to test */
        if (cand == 2)
            next_candidate = 3;      /* skip 2, start with first odd */
        else
            next_candidate += 2;     /* test only odd numbers */

        if (is_prime(cand)) {
            primes_push(cand); /* keep prime for future tests */
            if (cand % 7 == 0 || cand % 7 == 3 || cand % 7 == 4)
                cache_push((seq_t)cand);
        }
    }

    return cache[n - 1];
}
