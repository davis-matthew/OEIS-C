/*
 * A043802  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 4 mod 8.
 * https://oeis.org/A043802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   base3_runs_congruent4_mod8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = the n‑th integer whose number of runs in the base‑3
 * representation of n is congruent to 4 (mod 8).  Computed by brute‑force
 * search with memoization. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/base3_runs_congruent4_mod8.h"

/* Cached sequence data.  All static variables are used only inside this
   translation unit to accumulate computed terms up to the largest
   requested index. */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_test = 0;   /* next integer to be examined */

/* Return the n-th term (n >= 1) of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) {                /* offset is 1; undefined for 0 */
        return (seq_t)0;
    }

    /* Extend the cache until it contains the requested term. */
    while (cache_len < (size_t)n) {
        if (base3_runs_congruent4_mod8((seq_t)next_test)) {
            /* Ensure enough capacity. */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {          /* allocation failed */
                    return (seq_t)0; /* fallback */
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_test;
        }
        /* Move to the next integer to test. */
        ++next_test;
    }

    /* Sequence is 1‑indexed; return the stored term. */
    return cache[(size_t)n - 1];
}
