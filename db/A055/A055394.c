/*
 * A055394  Numbers that are the sum of a positive square and a positive cube.
 * https://oeis.org/A055394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 5420, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 8764 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Helper: recompute the list of terms up to newLimit.  */
static void recompute_terms(seq_t newLimit,
                            seq_t **cache, size_t *cacheSize)
{
    /* Bit array for marking representable numbers.  */
    unsigned char *present = (unsigned char *)
        calloc((size_t)(newLimit + 1), 1);
    if (present == NULL) return;   /* allocation failure */

    /* Generate all sums i^2 + j^3 ≤ newLimit.  */
    for (seq_t j = 1;; ++j) {
        seq_t cube = j * j * j;
        if (cube > newLimit) break;
        for (seq_t i = 1;; ++i) {
            seq_t square = i * i;
            if (square > newLimit - cube) break;
            seq_t val = cube + square;
            present[(size_t)val] = 1;
        }
    }

    /* Count distinct results and allocate cache.  */
    size_t cnt = 0;
    for (size_t v = 1; v <= (size_t)newLimit; ++v)
        if (present[v]) ++cnt;

    seq_t *newCache = mem_new(seq_t, cnt);
    size_t pos = 0;
    for (size_t v = 1; v <= (size_t)newLimit; ++v)
        if (present[v]) newCache[pos++] = (seq_t)v;

    free(present);

    /* Replace old cache.  */
    mem_free(*cache);
    *cache     = newCache;
    *cacheSize = cnt;
}

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
