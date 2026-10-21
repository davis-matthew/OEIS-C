/*
 * A215359  Primes congruent to {0, 2, 3, 4} mod 13.
 * https://oeis.org/A215359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p % 13 ∈ {0, 2, 3, 4} (the residue 0 occurs only for p = 13). */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"        /* for is_prime(idx_t) */
#include "../utils/grow_seq_array.h" /* for grow_seq_array(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* Undefined for n = 0, but safe to return 0. */

    /* Static cache for cached terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;           /* Number of terms currently cached. */
    static size_t cache_cap  = 0;           /* Allocated capacity of the cache. */
    static idx_t next_candidate = 2;       /* Next integer to test for primality. */

    /* Ensure that at least n terms are present in the cache. */
    while (cache_len < (size_t)n) {
        /* Find the next admissible prime. */
        for (;;) {
            idx_t cand = next_candidate;   /* Candidate to test. */
            ++next_candidate;              /* Prepare for the next call. */

            if (cand < 2) continue;         /* Skip 0 and 1. */

            /* Check the residue condition. */
            uint64_t r = cand % 13;         /* Safe because 13 fits in 64 bits. */
            bool ok = false;
            if (cand == 13) {
                ok = true;                  /* 13 itself is allowed. */
            } else if (r == 2 || r == 3 || r == 4) {
                ok = true;                  /* Residues 2,3,4 are acceptable. */
            }
            if (!ok) continue;              /* Not in the allowed residue set. */

            /* If the candidate passes the primality test, store it. */
            if (is_prime(cand)) {
                /* Resize cache if necessary. */
                if (cache_len == cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    grow_seq_array(&cache, &cache_cap, new_cap);
                }
                cache[cache_len++] = (seq_t)cand;
                break;                      /* Go back to the outer while-loop. */
            }
        }
    }

    return cache[n - 1]; /* a(1) corresponds to cache[0]. */
}
