/*
 * A115334  Numbers d > 0 such that 3+2d and 3+4d are primes.
 * https://oeis.org/A115334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th positive integer d such that 3+2d and 3+4d are
 *            both prime
 * Range:     u8 n <= 44, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

/* Cache for computed terms of the sequence. */
static seq_t *cache     = NULL; /* array of found d values */
static size_t cache_cap = 0;    /* allocated capacity of cache     */
static size_t cache_len = 0;    /* number of values currently stored */
static idx_t   next_d   = 0;    /* next d to test (starts at 0 so that the first tested d is 1) */

/* Ensure that cache contains at least n terms. */
static void build_cache_to(idx_t n)
{
    while (cache_len < n) {
        ++next_d; /* candidate d (d > 0) */

        idx_t p1 = 3 + 2 * next_d; /* 3 + 2d */
        idx_t p2 = 3 + 4 * next_d; /* 3 + 4d */

        if (is_prime(p1) && is_prime(p2)) {
            /* Grow cache if necessary */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
                if (!tmp) { /* allocation failure – abort */
                    exit(EXIT_FAILURE);
                }
                cache     = tmp;
                cache_cap = new_cap;
            }
            /* Store the found d as a seq_t value */
            cache[cache_len++] = (seq_t)next_d;
        }
    }
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* sequence is 1‑based – behaviour for n < 1 is irrelevant */
    build_cache_to(n);
    return cache[n - 1];
}
