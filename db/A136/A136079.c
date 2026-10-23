/*
 * A136079  Primes of the form 21*p + 20 with p prime.
 * https://oeis.org/A136079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 192, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime of the form 21*p + 20 with p prime. This generates the n‑th
   term by searching for primes p sequentially and testing
   q = 21*p + 20 for primality. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Cache of previously computed terms. */
static seq_t *cache = NULL;          /* array of results          */
static idx_t  cache_cap = 0;         /* current allocated size    */
static idx_t  cache_len = 0;         /* number of elements stored */

/* Next integer to be tested as a candidate prime p. */
static uint64_t next_candidate = 2;

/* Ensure the cache has room for one more element. */
static void cache_grow(void) {
    if (cache_len == cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1U : 64;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return;               /* allocation failure – abort silently */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Return the n‑th term (n >= 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined but harmless */

    /* Generate terms until we have at least n. */
    while (cache_len < n) {
        /* Find next prime p. */
        while (!is_prime_small((seq_t)next_candidate))
            ++next_candidate;

        /* Compute q = 21*p + 20. */
        uint64_t p = next_candidate;
        seq_t q = (seq_t)p * (seq_t)21 + (seq_t)20;

        /* Keep q if it is prime. */
        if (is_prime_big(q)) {
            cache_grow();
            cache[cache_len++] = q;
        }

        /* Advance to the next candidate for p. */
        ++next_candidate;
    }

    return cache[n - 1];
}
