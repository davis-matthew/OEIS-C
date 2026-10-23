/*
 * A142138  Primes congruent to 29 mod 37.
 * https://oeis.org/A142138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime p such that p % 37 == 29
 * Range:     u8 n <= 3, u16 n <= 190, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by incremental sieve with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* util function: primality test for unsigned integer */
#include "../utils/is_prime.h"

static seq_t *prime_cache = NULL;
static size_t cache_used = 0;   /* number of elements stored */
static size_t cache_cap  = 0;   /* allocated capacity */
static seq_t last_candidate = 0;/* last checked candidate */

/* Ensure sufficient capacity, grow if needed */
static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    seq_t *newp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    if (!newp) {
        /* Allocation failed */
        abort();
    }
    prime_cache = newp;
    cache_cap = new_cap;
}

/* Store a new prime into the cache */
static void cache_push(seq_t val)
{
    if (cache_used == cache_cap) {
        cache_grow();
    }
    prime_cache[cache_used++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) { /* undefined for n<1; return 0 sentinel */
        return (seq_t)0;
    }

    /* Extend cache until we have at least n elements */
    while (cache_used < n) {
        if (cache_used == 0) {
            /* First prime */
            last_candidate = 29;
        } else {
            last_candidate += 37;
        }
        /* Test primality */
        if (is_prime((idx_t)last_candidate)) {
            cache_push(last_candidate);
        }
    }
    return prime_cache[n - 1];
}
