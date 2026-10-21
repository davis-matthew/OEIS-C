/*
 * A235622  Primes whose base-8 representation also is the base-7
 *          representation of a prime.
 * https://oeis.org/A235622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 387, u32 n <= 301, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   base7_to_base8, is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth prime p such that its base‑8 representation equals
   the base‑7 representation of some prime q.  The function builds the list
   lazily using a simple prime sieve for q and converts each prime q to a
   base‑8 number p; if p itself is prime it is appended to the cache.  The
   result is returned in OEIS indexing (a(1) is the first term).  The
   implementation works for all supported seq_t sizes (including
   arbitrary‑precision) and does not hard‑code any terms.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"        /* bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t) */
#include "../utils/base7_to_base8.h"

static seq_t *cache = NULL;      /* cached sequence values */
static size_t cache_len  = 0;    /* number of stored terms */
static size_t cache_cap  = 0;    /* allocated capacity */
static idx_t  next_q     = 2;    /* next integer to test as prime q */

/* Ensure that the cache can hold at least one more element. */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 256 : cache_cap << 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* out of memory – abort the program */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Append a new term to the cache. */
static void cache_append(seq_t val)
{
    ensure_cache_capacity();
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0 – return 0 as a safe value */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        /* Find the next prime q */
        while (!is_prime(next_q)) {
            next_q++;
        }

        /* Compute candidate p from this q */
        seq_t p = base7_to_base8(next_q);

        /* If p is prime, cache it */
        if (is_prime_big(p)) {
            cache_append(p);
        }

        /* Prepare for the next iteration */
        next_q++;
    }

    return cache[n - 1];
}
