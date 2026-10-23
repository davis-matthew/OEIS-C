/*
 * A125225  Numbers n such that n-1 can be represented as a sum of a subset of
 *          divisors of n.
 * https://oeis.org/A125225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_subset_sum_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the nth integer n (n≥1) such that n-1 can be expressed as a sum
   of a subset of the proper divisors of n.  The function returns the
   value of the sequence at index n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_subset_sum_uint64.h"

/* Static cache of found sequence values */
static seq_t *found      = NULL;
static size_t foundsize  = 0U;   /* number of elements stored */
static size_t foundcap   = 0U;   /* allocated capacity */

/* Next candidate integer to test (starting from 1). */
static uint64_t next_candidate = 1U;

/* Sequence function requested by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but avoid crash */

    size_t nidx = (size_t)n;       /* index in 0‑based array */

    /* Generate terms until requested index is reached. */
    while (foundsize < nidx) {
        uint64_t cand = next_candidate++;
        if (has_subset_sum_uint64(cand)) {
            /* Ensure capacity for found array. */
            if (foundsize >= foundcap) {
                size_t newcap = foundcap ? foundcap * 2ULL : 16ULL;
                seq_t *tmp = (seq_t*)realloc(found, newcap * sizeof(seq_t));
                if (!tmp) break;        /* allocation failure */
                found = tmp;
                foundcap = newcap;
            }
            found[foundsize++] = (seq_t)cand;
        }
    }

    /* Return cached value. */
    return found[nidx - 1U];
}
