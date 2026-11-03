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
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper that checks whether the base‑5 representation of x contains
 * the consecutive digits 1 followed by 2 (from most‑significant to
 * least‑significant). */
static inline bool has_sub12(seq_t x)
{
    unsigned char digits[64];  /* enough for any 128‑bit value */
    size_t len = 0u;

    if (x == 0)            /* zero has no substring 12 */
        return false;

    while (x > 0) {
        digits[len++] = (unsigned char)(x % 5);
        x = x / 5;
    }

    /* digits[0] is least significant, digits[len-1] most; scan most‑>least */
    for (size_t i = len; i > 1u; --i) {
        if (digits[i-1] == 1 && digits[i-2] == 2)
            return true;
    }
    return false;
}

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
            seq_t *tmp = mem_resize(seq_t, cache, newCap);
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
