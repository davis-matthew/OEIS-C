/*
 * A178550  Primes with exactly one digit 1.
 * https://oeis.org/A178550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 2543, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_one_digit_one, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the nth prime whose decimal representation
 * contains exactly one digit '1'.
 *
 * The algorithm generates primes on demand, keeping a static cache
 * of the discovered terms.  It uses the library routine
 * "next_prime(uint64_t start)" to obtain the successive primes.
 * A helper checks whether a prime contains a single decimal digit
 * equal to one.  All arithmetic is performed in seq_t; the generator
 * works with 64‑bit integers and will fail if a prime exceeds 64 bits,
 * but the OEIS b‑file for this sequence fits in 64 bits for the tested
 * indices.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* next_prime: returns the next prime > start (library function) */
#include "../utils/next_prime.h"
#include "../utils/has_one_digit_one.h"

/* --------------------------------------------------------------------- */
/* Helper: test whether a number has exactly one decimal digit '1'.      */

/* --------------------------------------------------------------------- */
/* Static cache of the sequence terms.                                   */
/* --------------------------------------------------------------------- */
static seq_t *cache      = NULL;   /* array of found primes       */
static size_t cache_cnt  = 0;      /* number of elements stored   */
static size_t cache_cap  = 0;      /* current allocated capacity  */
static uint64_t last_checked = 1;  /* last number tested for prime */

/* --------------------------------------------------------------------- */
/* Append a value to the cache, expanding it if necessary.                */
/* --------------------------------------------------------------------- */
static void cache_append(seq_t val)
{
    if (cache_cnt == cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();             /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* --------------------------------------------------------------------- */
/* Generate terms until we have at least n elements in the cache.        */
/* --------------------------------------------------------------------- */
static void generate_terms(idx_t n)
{
    while (cache_cnt < n)
    {
        /* find next prime after last_checked               */
        uint64_t prime = next_prime(last_checked);
        /* update last_checked to just above the found prime */
        last_checked = prime + 1;

        /* if the prime contains exactly one digit '1', store it */
        seq_t sp = (seq_t)prime;   /* cast to seq_t for the check */
        if (has_one_digit_one(sp))
            cache_append(sp);
    }
}

/* --------------------------------------------------------------------- */
/* Public function: returns a(n) for n >= 1.                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for n < 1 */

    generate_terms(n);                   /* ensure enough terms */
    return cache[n - 1];
}
