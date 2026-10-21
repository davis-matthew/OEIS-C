/*
 * A201477  Primes of the form 3n^2 + 4.
 * https://oeis.org/A201477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 + 3*A111052(n)^2
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 3194, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns the n-th prime of the form 3*m^2 + 4 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;     /* cached primes */
static size_t cache_len       = 0;   /* number of primes stored */
static size_t cache_cap       = 0;   /* capacity of 'cache' */
static idx_t  last_m          = 0;   /* largest 'm' that has been tested */

/* Append a new prime to the cache, growing the array if necessary */
static void append_prime(seq_t p)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 2 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            /* out of memory; abort */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = p;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* not defined for n < 1 */

    /* Generate primes until we have at least n of them */
    while (cache_len < n) {
        last_m++;                           /* next integer to test */
        seq_t m_sq = (seq_t)last_m * (seq_t)last_m;
        seq_t p    = 3 * m_sq + 4;           /* 3*m^2 + 4 */

        /* Test primality of p */
        if (is_prime((idx_t)p)) {
            append_prime(p);
        }
    }

    /* Return the n-th prime (1-based index) */
    return cache[n - 1];
}
