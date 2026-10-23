/*
 * A044656  Numbers n such that string 2,6 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer such that its base‑9
 *            representation contains the substring “26” while the base‑9
 *            representation of the integer plus one does not
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   base9_has_26
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/base9_has_26.h"

/* Cache of already computed terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;     /* number of terms stored */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_candidate = 0; /* next number to test */

/* Function that returns the n-th term of sequence A044656. */
seq_t a(idx_t n)
{
    /* Convert idx_t to size_t safely for array indexing. */
    size_t target = (size_t)n;

    /* Grow cache to store at least target terms. */
    while (cache_len < target) {
        /* Find next candidate satisfying the property. */
        seq_t x = next_candidate;
        for (;; ++x) {
            if (base9_has_26(x) && !base9_has_26(x + 1)) {
                /* Store found term. */
                if (cache_len >= cache_cap) {
                    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
                    seq_t *newp = (seq_t *)realloc(cache,
                                                    new_cap * sizeof(seq_t));
                    cache = newp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = x;
                next_candidate = x + 1;
                break;
            }
        }
    }

    return cache[target - 1]; /* a(1) is at index 0 */
}
