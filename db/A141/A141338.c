/*
 * A141338  Primes of the form x^2+9*x*y-3*y^2 (as well as of the form
 *          7*x^2+11*x*y+y^2).
 * https://oeis.org/A141338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 19, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   append_candidate, floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A141338: primes that can be written as  
   x^2 + 9*x*y - 3*y^2  or  7*x^2 + 11*x*y + y^2.  */
/* The function searches primes and keeps a cache so that repeated
   calls are O(1) after the first few terms. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/is_prime.h"          /* bool is_prime(idx_t); */
#include "../utils/append_candidate.h"  /* void append_candidate(seq_t **p,
                                            size_t *len, size_t *cap, seq_t val); */
#include "../utils/floor_sqrt.h"        /* seq_t floor_sqrt(seq_t x); */

static seq_t *cache = NULL;      /* Array of found primes */
static size_t cache_len = 0;     /* Number of stored primes */
static size_t cache_cap = 0;     /* Capacity of the array */

static idx_t current_candidate = 2;   /* Next integer to test for primality */

/* Test whether a prime p can be represented by one of the required forms.
   p is passed as a seq_t so that the algorithm works for arbitrary-precision
   builds. */
static bool rep_form(seq_t p)
{
    /* Primes smaller than 7 are not in the sequence. */
    if (p < 7)
        return false;

    /* Upper bound for y: floor(sqrt(p/3)) */
    seq_t maxy = floor_sqrt(p / (seq_t)3);

    for (seq_t y = 1; y <= maxy; ++y) {
        seq_t D = 93 * y * y + 4 * p;           /* Discriminant */
        seq_t sqrtD = floor_sqrt(D);

        if (sqrtD * sqrtD != D)
            continue;                          /* D not a perfect square */

        seq_t sum = sqrtD + 9 * y;
        if (sum & (seq_t)1)                     /* must be even to give integer x */
            continue;

        return true;                            /* representation found */
    }
    return false;
}

/* Return the n-th prime of the sequence (n >= 1). */
seq_t a(idx_t n)
{
    /* Return cached value if available. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Extend the cache until we have at least n entries. */
    while (cache_len < n) {
        /* Find next prime candidate. */
        while (!is_prime(current_candidate))
            ++current_candidate;

        /* Store the prime if it satisfies the representation. */
        if (rep_form((seq_t)current_candidate))
            append_candidate(&cache, &cache_len, &cache_cap,
                             (seq_t)current_candidate);

        ++current_candidate;
    }

    return cache[n - 1];
}
