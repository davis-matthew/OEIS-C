/*
 * A179540  a(0)=0, a(1)=1, a(n)=2*n*(a(n-1)+a(n-2)), n>1.
 * https://oeis.org/A179540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(a(n-1)+a(n-2)), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation.  The first element is a(0),
   the second is a(1).  All subsequent elements are computed
   on demand. */
static seq_t *cache = NULL;   /* array of computed values */
static idx_t   cap   = 0;     /* capacity of the array */
static idx_t   last  = 0;     /* highest index already computed */

/* Allocate or grow the cache so that indices up to n are stored.
   After this call, cache[n] is defined.  The routine is called from
   a() only, so no external synchronization is needed. */
static void
ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: at least n+1 entries. */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t*)malloc(sz * sizeof(seq_t));
        if (cache == NULL) exit(1);          /* out of memory */
        cap   = sz;
        cache[0] = 0;
        if (sz > 1) cache[1] = 1;
        last  = (sz > 1 ? 1 : 0);
    } else if (cap < (size_t)(n + 1)) {
        /* Grow the allocation. */
        size_t sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(cache, sz * sizeof(seq_t));
        if (tmp == NULL) exit(1);            /* out of memory */
        cache = tmp;
        cap   = sz;
        /* Existing values are preserved.  New slots will be filled. */
    }

    /* Compute unsatisfied values. */
    if (last < n) {
        for (idx_t i = last + 1; i <= n; ++i) {
            cache[i] = 2 * (seq_t)i * (cache[i - 1] + cache[i - 2]);
        }
        last = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    ensure_cache(n);
    return cache[n];
}
