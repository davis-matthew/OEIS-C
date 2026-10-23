/*
 * A175227  Sequence of the remaining nonprimes out of sieve of type: {step 1:
 *          starting with the sequence of positive integers A000027; step 2:
 *          delete every prime number A000040; step 3: delete every
 *          prime(1)-th number greater than the prime(1) from the remaining
 *          sequence; step 4: delete every prime(2)-th nu...
 * https://oeis.org/A175227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   compute_sequence
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A175227 (OEIS)
   Sequence of remaining nonprimes after progressively deleting every
   p‑th element greater than p for each prime p (p = 2,3,5,7,…).  The
   implementation builds the list of surviving numbers up to a dynamic
   upper bound until it contains at least the requested term index. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/compute_sequence.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached survivors */
    static size_t cache_len = 0;
    static idx_t cache_limit = 0;

    if (n == 0) return (seq_t)0;   /* undefined for 0 but safe */

    /* Rebuild sequence if we don't yet have enough terms */
    if (cache_len < (size_t)n) {
        idx_t limit = cache_limit ? cache_limit
                                  : ((n > 100) ? n * 10 : 100);
        if (limit < n) limit = n * 10;

        while (true) {
            size_t len;
            seq_t *newc = compute_sequence(limit, &len);
            if (!newc) return (seq_t)0;   /* allocation failure */

            if (cache) free(cache);
            cache = newc;
            cache_len = len;
            cache_limit = limit;

            if (cache_len >= (size_t)n) break;
            limit *= 2;                    /* increase bound and retry */
        }
    }

    return cache[n - 1];
}
