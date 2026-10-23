/*
 * A033225  Primes of form x^2+37*y^2.
 * https://oeis.org/A033225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1358, u32 n <= 1359, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   alloc_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A033225: primes p of the form x^2 + 37*y^2
 * Equivalently, primes ≡ {1,9,21,25,33,37,41,49,53,65,73,77,81,85,101,121,137,141,145} (mod 148) */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"          /* is_prime_big(seq_t) */
#include "../utils/alloc_seq.h"            /* seq_alloc, seq_realloc etc. */

static const int residues[19] = {
    1, 9, 21, 25, 33, 37, 41, 49, 53,
    65, 73, 77, 81, 85, 101, 121, 137, 141, 145
};

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------------
     * Static cache of found primes so far.  The cache grows automatically
     * whenever the requested index exceeds the current size.
     * ------------------------------------------------------------------ */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of elements stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static uint64_t next_candidate = 37;   /* next integer to test for primality */

    /* Allocate or enlarge the cache if necessary */
    if (cache_size < (size_t)n) {
        /* Ensure we have enough space for the new elements */
        while (cache_size < (size_t)n) {
            /* Test the current candidate for primality */
            seq_t cand = (seq_t)next_candidate;
            if (is_prime_big(cand)) {
                /* Check if the prime has one of the allowed residues */
                int r = (int)(next_candidate % 148);
                for (int i = 0; i < 19; ++i) {
                    if (r == residues[i]) {
                        /* Add to cache, reallocating if needed */
                        if (cache_size == cache_cap) {
                            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                            if (!tmp) {    /* allocation failed – abort gracefully */
                                return 0;
                            }
                            cache = tmp;
                            cache_cap = new_cap;
                        }
                        cache[cache_size++] = cand;
                        break;
                    }
                }
            }
            /* Advance to the next integer */
            ++next_candidate;
        }
    }

    /* Return the cached value (a is 1‑based indexing) */
    return cache[n - 1];
}
