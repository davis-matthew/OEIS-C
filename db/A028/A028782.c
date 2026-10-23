/*
 * A028782  Nonsquares mod 69.
 * https://oeis.org/A028782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Nonsquares mod 69: compute by scanning integers and rejecting those
 * that are quadratic residues modulo 69.  The algorithm
 * stores the set of residues once and then uses linear search
 * (caching the results) to answer arbitrary calls efficiently.
 */
#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle offset 1.  For n <= 0 the result is undefined; return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Lazy static initialization of the residue table and cache. */
    static bool is_square[69];
    static bool table_ready = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static idx_t next_candidate = 2;   /* start searching from 2 */

    /* Build the set of quadratic residues modulo 69. */
    if (!table_ready) {
        for (idx_t i = 0; i < 69u; ++i) {
            is_square[(i * i) % 69u] = true;
        }
        table_ready = true;
    }

    /* Expand cache capacity as needed. */
    if (cache == NULL) {
        cache_cap = 64;  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend cache until it contains at least n terms. */
    while (cache_size < n) {
        /* Find the next nonsquare. */
        while (true) {
            /* Compute modulus 69; cast 69 to seq_t for division. */
            int mod = (int)(next_candidate % (seq_t)69);
            if (!is_square[mod]) {
                /* Store the found nonsquare. */
                if (cache_size == cache_cap) {
                    cache_cap <<= 1;  /* double capacity */
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_size++] = next_candidate;
                break;
            }
            ++next_candidate;
        }
        ++next_candidate;  /* prepare for next iteration */
    }

    return cache[n - 1];
}
