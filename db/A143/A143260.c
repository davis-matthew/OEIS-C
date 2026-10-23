/*
 * A143260  Primes that are not emirps.
 * https://oeis.org/A143260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_small, rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that is not an emirp (prime whose reverse is a
   different prime).  The function generates primes on demand by
   incremental trial division and stores all primes that are not
   emirps in a static cache so that subsequent calls are fast. */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/rev.h"
#include "../utils/ensure_capacity.h"

/* Square array of primes that are not emirps */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated capacity */
static size_t cache_len = 0;   /* number of stored terms */

/* Next candidate for the prime search */
static uint64_t prime_start = 2;

/* Return the nth term (1‑based).  For n == 0 the behaviour is undefined
   but the function returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Return cached value if we have already computed enough terms. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate primes until we have at least n non‑emirps. */
    while (cache_len < n) {
        /* Find the next prime. */
        uint64_t p = prime_start;
        while (!is_prime_small((seq_t)p))
            ++p;
        prime_start = p + 1;

        /* Check whether this prime is an emirp. */
        uint64_t revp = rev(p);
        bool emirp = (p != revp && is_prime_small((seq_t)revp));

        if (!emirp) {   /* store non‑emirp prime */
            size_t needed = cache_len + 1;
            if (needed > cache_cap)
                ensure_capacity(needed, &cache_cap, (void **)&cache, sizeof(seq_t));

            cache[cache_len++] = (seq_t)p;
        }
    }

    return cache[n - 1];
}
