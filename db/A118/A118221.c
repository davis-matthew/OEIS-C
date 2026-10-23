/*
 * A118221  Start with 1 and repeatedly reverse the digits and add 73 to get
 *          the next term.
 * https://oeis.org/A118221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, less
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n) = rev_decimal(a(n-1)) + 73 for n > 1. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/rev_decimal.h"

/* Static cache for memoization.
 * cache[i] holds a(i+1) (zero‑based index). */
static seq_t *cache      = NULL;
static idx_t  cache_size = 0;   /* allocated number of slots   */
static idx_t  cache_len  = 0;   /* number of terms already computed */

/* Ensure that at least 'needed' slots are allocated for the cache. */
static void
ensure_cache(idx_t needed)
{
    if (needed <= cache_size)
        return;

    /* Reallocate to the requested size. The cast to size_t is safe
     * for the expected ranges of n (up to ~10,000) and for 64‑bit
     * indexes. */
    seq_t *p = (seq_t *)realloc(cache, (size_t)needed * sizeof(seq_t));
    cache      = p;
    cache_size = needed;
}

/* Return the n‑th term of A118221.  n is 1‑based.  For n < 1 return 0. */
seq_t
a(idx_t n)
{
    if (n < 1)               /* sequence undefined for n < 1 */
        return (seq_t)0;

    /* Make sure the cache can hold at least n terms */
    ensure_cache(n);

    /* Bootstrap the first value if the cache is empty. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache_len = 1;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len; i < n; ++i) {
        seq_t rev = rev_decimal(cache[i - 1]);   /* a(i) = rev(a(i-1)) + 73 */
        cache[i]  = rev + (seq_t)73;
    }
    cache_len = n;   /* now all indices < n are valid */

    return cache[n - 1];   /* a(n) is stored at zero‑based index n-1 */
}
