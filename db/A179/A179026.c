/*
 * A179026  Primes whose only prime digits are 3's.
 * https://oeis.org/A179026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 814, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   acceptable_digits, is_prime_generic
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th prime whose decimal digits are all
   from {0,1,3,4,6,8,9} and which contains at least one digit 3. */

/* This implementation uses a linear search for primes, testing
   each odd candidate in turn.  It caches results so that calls to
   a() with arbitrary ordering of n are handled efficiently. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_generic.h"
#include "../utils/acceptable_digits.h"

static seq_t *cache      = NULL;   /* stored sequence values */
static size_t cache_sz   = 0;      /* number of values stored */
static size_t cache_cap  = 0;      /* current capacity of cache */
static uint64_t last_tested = 1;   /* last tested odd integer */

/* Double the cache capacity, allocating from the heap. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2u : 4u;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) exit(EXIT_FAILURE);
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Generate the next term of the sequence and store it in the cache. */
static void generate_next(void)
{
    uint64_t cand = last_tested + 2;  /* skip even numbers */
    for (;;) {
        if (acceptable_digits(cand) && is_prime_generic(cand)) {
            if (cache_sz == cache_cap) grow_cache();
            cache[cache_sz++] = (seq_t)cand;
            last_tested = cand;
            return;
        }
        cand += 2;
    }
}

/* Return the n-th element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)0;       /* undefined for n<1 – safe return */

    /* Ensure we have computed up to the requested index. */
    while (cache_sz < (size_t)n)
        generate_next();

    return cache[(size_t)(n - 1u)];
}
