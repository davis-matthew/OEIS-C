/*
 * A128635  Number of numbers not greater than n and divisible by all digits
 *          in decimal representation.
 * https://oeis.org/A128635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1790, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_nonn
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A128635: Number of numbers ≤ n and divisible by all digits of their decimal representation.
   Formula: a(n) = |{1 ≤ k ≤ n : for every decimal digit d of k, d ≠ 0 and k mod d = 0}|.
   The function incrementally builds a cache of cumulative counts.
*/

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_nonn.h"

/* Cache of cumulative counts: cache[i] = a(i); cache[0] = 0. */
static seq_t *cache     = NULL;   /* cache array */
static size_t cache_cap = 0;      /* allocated capacity */
static idx_t  cached_last = 0;    /* highest n previously computed */

/* a(n): number of integers ≤ n divisible by all their decimal digits. */
seq_t a(idx_t n)
{
    if (n == 0)            /* defined only for n ≥ 1 */
        return 0;

    /* Ensure cache array is large enough. */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;                     /* +1 for index 0 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)                                    /* allocation failure */
            return 0;
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base case if this is the first call. */
    if (cached_last == 0)
        cache[0] = 0;

    /* Compute values for any unseen indices up to n. */
    for (idx_t i = cached_last + 1; i <= n; ++i) {
        seq_t inc = is_nonn((seq_t)i) ? 1 : 0;
        cache[i] = cache[i - 1] + inc;
    }
    cached_last = n;

    return cache[n];
}
