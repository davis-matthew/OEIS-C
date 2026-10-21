/*
 * A078057  Expansion of (1+x)/(1-2*x-x^2).
 * https://oeis.org/A078057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0)=1, a(1)=3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* A simple memoizing implementation.  The global cache grows as needed
 * to handle any requested n.  This implementation is safe for any
 * seq_t type (even arbitrary‑precision) because it only performs the
 * recurrence operations on values already stored in the cache. */

static seq_t *a_cache      = NULL;   /* cache storage */
static idx_t  a_cache_len  = 0;      /* number of elements actually stored   */
static idx_t  a_cache_cap  = 0;      /* size of allocated buffer (in seq_t's) */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0u)      return (seq_t)1;
    if (n == 1u)      return (seq_t)3;
    /* ensure the cache has enough capacity */
    if (a_cache_cap <= n) {
        idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 16;
        if (new_cap <= n) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        /* On failure to reallocate we let the program abort (as usual for
           the OEIS evaluators); this case should not occur for the target
           limits. */
        a_cache = tmp;
        a_cache_cap = new_cap;
    }
    /* initialise cache if not yet done */
    if (a_cache_len == 0) {
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)3;
        a_cache_len = 2;
    }
    /* compute and store further values up to n */
    while (a_cache_len <= n) {
        seq_t a_prev   = a_cache[a_cache_len - 1];
        seq_t a_prev2  = a_cache[a_cache_len - 2];
        a_cache[a_cache_len] = (2 * a_prev) + a_prev2;
        ++a_cache_len;
    }
    return a_cache[n];
}
