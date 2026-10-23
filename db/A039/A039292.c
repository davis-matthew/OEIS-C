/*
 * A039292  Numbers whose base-5 representation has the same nonzero number of
 *          3's and 4's.
 * https://oeis.org/A039292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑5 representation has the same nonzero number
 *            of 3's and 4's
 * Range:     u8 n <= 36, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   append_candidate, has_equal_nonzero_3s_and_4s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/append_candidate.h"
#include "../utils/has_equal_nonzero_3s_and_4s.h"

/* Memoised storage of the sequence. */
static seq_t *cache          = NULL;  /* array of sequence values */
static size_t cache_len      = 0;      /* number of values stored */
static size_t cache_cap      = 0;      /* allocated capacity */

/* State used while scanning natural numbers. */
static seq_t   last_tested  = 0;      /* last number examined (>=1) */
static idx_t   matched_cnt  = 0;      /* how many sequence terms found */

/* Return the n-th term, 1‑based. */
seq_t a(idx_t n)
{
    if (n <= 0) return 0;  /* undefined for n <= 0, but guard */

    /* If we already have the term, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* Generate more terms until we reach index n. */
    while (matched_cnt < n) {
        ++last_tested;  /* next natural number */

        if (has_equal_nonzero_3s_and_4s(last_tested)) {
            /* Store the new term. */
            append_candidate(&cache, &cache_len, &cache_cap, last_tested);
            ++matched_cnt;
        }
    }

    return cache[(size_t)n - 1];
}
