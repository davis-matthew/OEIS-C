/*
 * A144291  Triangular numbers n*(n-1)/2 with n and n -1 nonprime.
 * https://oeis.org/A144291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 219, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144291: a(n) = n*(n-1)/2 where n and n-1 are nonprime (triangular numbers of integers with consecutive composites). */
/* The function builds the sequence by testing successive integers for non‑primality and memoizing the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* static state for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    static idx_t next_candidate = 1;  /* next integer to test for inclusion */

    if (n == 0) return 0;  /* offset 1: a(1) is first term, n<1 not used */

    /* Expand cache if necessary */
    if (n > cache_len) {
        /* Ensure cache can hold at least n elements */
        if (cache_cap < n) {
            idx_t new_cap = cache_cap ? cache_cap * 2 : 64;
            if (new_cap < n) new_cap = n;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(*cache));
            if (!tmp) return 0;   /* allocation failure; caller should handle */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Generate terms until we reach the required index */
        while (cache_len < n) {
            /* Find next integer where both it and its predecessor are nonprime */
            while (is_prime(next_candidate) || is_prime(next_candidate - 1))
                ++next_candidate;

            /* Compute triangular number n*(n-1)/2 */
            seq_t val = (seq_t)next_candidate * ((seq_t)next_candidate - 1) / 2;

            /* Store the value */
            cache[cache_len++] = val;

            /* Advance to the next integer for subsequent searches */
            ++next_candidate;
        }
    }

    /* Return the memoized value for the requested index (offset 1) */
    return cache[n - 1];
}
