/*
 * A074781  Primes of the form p*2^k + 1 for any k and any prime p.
 * https://oeis.org/A074781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 1049, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   isPrime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074781: primes p such that (p‑1)/gpf(p‑1) is a power of two
   (equivalently primes of the form q*2^k+1 for some prime q and k≥0).
   This implementation searches primes one by one and caches the
   qualifying terms. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrime.h"
#include "../utils/next_prime.h"

/* Return the n‑th term (1‑based) of A074781 */
seq_t a(idx_t n)
{
    /* Cache for already computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* The smallest prime that has not yet been tested */
    static uint64_t next_to_check = 2;   /* start after 2 so 2 itself is skipped */

    /* Keep computing until we have at least n terms */
    while (cache_len < (size_t)n) {
        /* Find the next prime */
        uint64_t p = next_prime(next_to_check);
        next_to_check = p;              /* update for the next search */

        /* p is always odd here (next_prime skips 2 when starting at 2) */

        /* Remove all factors of 2 from p‑1 */
        uint64_t odd = p - 1;
        while ((odd & 1ULL) == 0ULL) odd >>= 1;

        /* Condition: odd part is 1 (p‑1 is a power of 2) or odd part is prime */
        if (odd == 1ULL || isPrime((idx_t)odd)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!tmp) { /* Allocation failure – terminate abruptly */
                    abort();
                }
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
