/*
 * A046304  Divisible by at least 5 primes (counted with multiplicity).
 * https://oeis.org/A046304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046304: numbers with at least 5 prime factors (counted with multiplicity).
 * The sequence a(n) is the n‑th integer m with Ω(m) ≥ 5, where Ω(m) is the
 * total number of prime factors of m counted with multiplicity.
 * The implementation uses a simple incremental sieve that keeps a cache of
 * already found values.  It re‑allocates the cache with a helper provided
 * by the OEIS utilities. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  For n==0 the argument is undefined. */
    if (n == 0)
        return 0;

    /* Static cache of computed terms. */
    static seq_t *cache      = NULL;
    static size_t  cache_cap = 0;
    static size_t  cache_len = 0;

    /* Next integer to test for Ω>=5. */
    static seq_t next_candidate = 2;

    /* Return already cached value. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Grow the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Scan integers until a suitable one is found. */
        while (true) {
            if (bigomega((idx_t)next_candidate) >= 5) {
                /* Ensure enough space for the new value. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 64 : cache_cap * 2;
                    seq_t *new_ptr = seq_realloc(cache, cache_cap, new_cap);
                    if (!new_ptr)
                        abort();        /* out of memory */
                    cache      = new_ptr;
                    cache_cap  = new_cap;
                }
                cache[cache_len++] = next_candidate;
                /* Advance to the next integer to test. */
                next_candidate++;
                break;
            }
            next_candidate++;
        }
    }

    return cache[n - 1];
}
