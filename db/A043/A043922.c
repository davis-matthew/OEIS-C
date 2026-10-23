/*
 * A043922  Numbers k such that 2 and 3 occur juxtaposed in the base-7
 *          representation of k but not of k+1.
 * https://oeis.org/A043922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers k for which the digits 2 and 3 occur adjacent in
 *            the base‑7 representation of k in any order, but the same does
 *            not hold for k+1
 * Range:     u8 n <= 10, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   has_adj23or32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers compatible with C11 and C++17 */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adj23or32.h"

/* Cache for previously computed terms and state for incremental
   generation. */
static seq_t *cache = NULL;          /* array of cached terms */
static size_t cache_cap = 0;         /* current capacity of cache   */
static size_t cache_size = 0;        /* number of terms stored      */
static seq_t last_candidate = 0;     /* last integer examined       */

/* Grow the cache to hold at least `need` terms. */
static void grow_cache(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap;
    while (new_cap < need)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort as building continues
           without a cache would be impossible. */
        exit(1);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence routine.  O(1) after the required term has been
   discovered; otherwise incremental search from the last examined
   candidate. */
seq_t a(idx_t n)
{
    /* n is 1‑based according to OEIS offset 1 */
    if (n == 0)
        return (seq_t)0;            /* undefined, but safe */

    /* If we already have the term cached, return it. */
    if (n <= cache_size)
        return cache[n - 1];

    /* Ensure we have enough room. */
    grow_cache((size_t)n);

    /* Search until we have computed the n‑th term. */
    while (cache_size < (size_t)n) {
        last_candidate++;             /* next candidate to test */
        if (has_adj23or32(last_candidate) &&
            !has_adj23or32(last_candidate + (seq_t)1)) {
            cache[cache_size] = last_candidate;
            cache_size++;
        }
    }

    return cache[n - 1];
}
