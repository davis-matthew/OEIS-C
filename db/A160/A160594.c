/*
 * A160594  Indices of primes congruent to 1 modulo 12.
 * https://oeis.org/A160594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A160594: indices of primes ≡ 1 (mod 12) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- static state ----------------------------------------------------------- */
static uint64_t *prime_list = NULL;    /* primes found so far (uint64_t) */
static size_t   prime_cap  = 0;        /* capacity of prime_list */
static size_t   prime_len  = 0;        /* number of primes stored */

static seq_t  *cache       = NULL;    /* cache[1..] holds a(n) values (1‑based) */
static size_t  cache_cap   = 0;        /* allocated capacity of cache */

static uint64_t next_candidate = 2;    /* next value to test for primality */
static idx_t    specials_found = 0;    /* count of primes ≡ 1 (mod 12) seen */

/* ensure prime_list can hold at least 'needed' elements */
static void ensure_prime_cap(size_t needed)
{
    if (needed <= prime_cap) return;
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_list) exit(EXIT_FAILURE);
    prime_list = new_list;
    prime_cap  = new_cap;
}

/* ensure cache can hold at least 'needed' indices (1‑based) */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;
}

/* primality test using all primes found so far */
static bool is_prime(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t i = 0; i < prime_len; ++i) {
        uint64_t p = prime_list[i];
        if (p > cand / p) break;          /* p*p > cand */
        if (cand % p == 0) return false;
    }
    return true;
}

/* ---------------------------------------------------------------------------
   Main sequence function
   --------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    /* ensure cache can hold value n */
    if ((size_t)n > cache_cap) ensure_cache_cap((size_t)n);

    /* generate primes until we have seen n special primes */
    while (specials_found < n) {
        /* find next prime */
        uint64_t cand = next_candidate;
        while (!is_prime(cand)) ++cand;

        /* record prime */
        ensure_prime_cap(prime_len + 1);
        prime_list[prime_len++] = cand;

        /* update cache if it is ≡ 1 (mod 12) */
        if (cand % 12 == 1) {
            ++specials_found;
            ensure_cache_cap((size_t)specials_found);
            cache[specials_found] = (seq_t)prime_len;   /* store index */
        }

        next_candidate = cand + 1;   /* continue scanning */
    }

    return cache[n];
}
