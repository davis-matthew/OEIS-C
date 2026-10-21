/*
 * A215306  Primes congruent to {1, 2, 3, 5} mod 11.
 * https://oeis.org/A215306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime congruent to 1, 2, 3 or 5 modulo 11,
   using a simple sieve of Eratosthenes.  The result is cached so that
   repeated calls are efficient, and the code is valid both in C11 and
   C++17. */

/* include standard headers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* static cache of the filtered primes */
static seq_t *good_primes = NULL;
static size_t good_cnt   = 0;
static size_t sieve_lim  = 0;

/* Build the list of good primes up to 'limit'.  The sieve is allocated
   and freed with explicit casts so that the code compiles in C++ as
   well. */
static void build_good_primes(size_t limit)
{
    /* Sieve of Eratosthenes */
    bool *comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (!comp) abort();            /* out of memory */
    comp[0] = comp[1] = true;
    for (size_t p = 2; p * p <= limit; ++p) {
        if (!comp[p]) {
            for (size_t k = p * p; k <= limit; k += p) {
                comp[k] = true;
            }
        }
    }

    /* Allocate an estimated capacity; grow as needed. */
    size_t cap   = limit / 10 + 10;
    seq_t *vec   = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!vec) abort();

    size_t cnt   = 0;
    for (size_t p = 2; p <= limit; ++p) {
        if (comp[p] || p == 11) continue;   /* skip non‑primes and 11 */
        unsigned r = (unsigned)(p % 11);
        if (r == 1 || r == 2 || r == 3 || r == 5) {
            if (cnt == cap) {                /* enlarge storage */
                cap <<= 1;                    /* double capacity */
                seq_t *tmp = (seq_t *)realloc(vec, cap * sizeof(seq_t));
                if (!tmp) abort();
                vec = tmp;
            }
            vec[cnt++] = (seq_t)p;           /* store good prime */
        }
    }

    free(comp);
    if (good_primes) free(good_primes);
    good_primes = vec;
    good_cnt    = cnt;
    sieve_lim   = limit;
}

/* Ensure that at least 'n' good primes are available in the cache. */
static void ensure_cache(idx_t n)
{
    if (!good_primes) {
        build_good_primes(300000);          /* initial small limit */
    }

    while ((idx_t)good_cnt < n) {
        size_t new_lim = sieve_lim * 2;
        if (new_lim <= sieve_lim)          /* protect against overflow */
            new_lim = sieve_lim + 100000;
        build_good_primes(new_lim);
    }
}

/* Return the n-th good prime (a(1) is the first). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined below offset */
    ensure_cache(n);
    return good_primes[n - 1];
}
