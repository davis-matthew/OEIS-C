/*
 * A055394  Numbers that are the sum of a positive square and a positive cube.
 * https://oeis.org/A055394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 5420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   recompute_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A055394  - Sum of a positive square and a positive cube
 *  The function returns the n-th smallest positive integer that can be
 *  represented as i^2 + j^3 with i,j ≥ 1.
 *
 *  The algorithm keeps an increasing bound 'limit'.  For a given limit
 *  we mark which integers ≤ limit can be written as a square plus a cube
 *  in a bit‑array, then collect the marked values into a sorted list.
 *  When more terms are required the limit is doubled and the process
 *  repeats until at least 'n' terms have been obtained.
 *
 *  All arithmetic uses only +,-,*,/,% and the integer types supplied
 *  by the build; no floating‑point or external library functions are
 *  required.  The routine works for any build where `seq_t` exceeds
 *  the maximum value produced (the arbitrary‑precision build is
 *  supported by using the same integer operations).
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/recompute_terms.h"

/* Public routine required by the framework.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* sorted distinct terms */
    static size_t cacheSize = 0;     /* number of cached terms   */
    static seq_t limit = 0;          /* maximal value cached    */

    /* If we already have enough terms, just return the answer.  */
    if (cacheSize >= (size_t)n) return cache[n - 1];

    /* Otherwise increase the bound until the cache contains n terms.  */
    seq_t newLimit = (cacheSize > 0) ? cache[cacheSize - 1] * (seq_t)2 : (seq_t)1;
    while (cacheSize < (size_t)n) {
        recompute_terms(newLimit, &cache, &cacheSize);
        if (cacheSize < (size_t)n) newLimit *= 2;
    }

    return cache[n - 1];
}
