/*
 * A044465  Numbers n such that string 1,2 occurs in the base 5 representation
 *          of n but not of n+1.
 * https://oeis.org/A044465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers n such that the base‑5 representation of n contains the
 *            * substring "12" but that of n+1 does not. The function returns
 *            the n‑th * term of OEIS sequence A044465 (offset 1)
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   has_sub12
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_sub12.h"

/* The main function to be called.  It maintains a static cache so that
 * repeated calls are fast, regardless of the order of the indices. */
seq_t a(idx_t n)
{
    /* Static state: cache of already computed terms and the next integer
     * candidate to test. */
    static seq_t *cache      = NULL;
    static size_t cacheSize  = 0;
    static size_t cacheCap   = 0;
    static seq_t nextCandidate = 0;

    /* Ensure we have at least n terms in the cache.  If not, continue
     * scanning from nextCandidate until we find more valid numbers. */
    while (cacheSize < (size_t)n) {
        seq_t cand = nextCandidate;
        seq_t cand_next = cand + 1;

        /* Iterate until we find a number that satisfies the
         * substring condition for cand but not for cand_next. */
        while (!(has_sub12(cand) && !has_sub12(cand_next))) {
            cand = cand + 1;
            cand_next = cand + 1;
        }

        /* Append the accepted candidate to the cache. */
        if (cacheSize == cacheCap) {
            size_t newCap = (cacheCap == 0u) ? 8u : cacheCap * 2u;
            seq_t *tmp = (seq_t *)realloc(cache, newCap * sizeof(seq_t));
            if (!tmp) {
                /* Allocation failed – abort gracefully. */
                exit(EXIT_FAILURE);
            }
            cache = tmp;
            cacheCap = newCap;
        }
        cache[cacheSize++] = cand;

        /* Advance the search start point. */
        nextCandidate = cand + 1;
    }

    /* Cache is large enough – return the requested term. */
    return cache[n-1];
}
