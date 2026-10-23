/*
 * A174166  Composites n such that 2*n-1 is prime.
 * https://oeis.org/A174166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th composite integer m such that (2*m-1) is prime
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Ensure the dynamic array has enough capacity */
static void ensure_seq_cap(size_t need, size_t *cap, seq_t **arr, size_t elem_size)
{
    if (need >= *cap) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (need >= new_cap) new_cap = need + 1;
        *arr = (seq_t *)realloc(*arr, new_cap * elem_size);
        *cap = new_cap;
    }
}

/* a(n) implementation */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* stored terms, 1‑based index */
    static size_t cache_size = 0;        /* number of terms currently cached   */
    static size_t cache_cap  = 0;        /* allocated capacity for cache        */
    static idx_t next_candidate = 2;     /* next integer to test for eligibility */

    if (n == 0) return (seq_t)0; /* undefined; safe fallback */

    /* Grow cache until it contains the requested term */
    while (cache_size < n) {
        /* Find next eligible composite */
        while (true) {
            /* Skip primes and 1 */
            if (next_candidate > 1 && !is_prime_big((seq_t)next_candidate)) {
                seq_t p = ((seq_t)2 * (seq_t)next_candidate) - (seq_t)1;
                if (is_prime_big(p)) {
                    /* Eligible – store it */
                    ensure_seq_cap(cache_size + 1, &cache_cap, &cache, sizeof(seq_t));
                    cache[cache_size++] = (seq_t)next_candidate;
                    ++next_candidate; /* advance after using it */
                    break;
                }
            }
            ++next_candidate;
        }
    }

    return cache[n - 1];
}
