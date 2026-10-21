/*
 * A061062  Sum of squared factorials: (0!)^2 + (1!)^2 + (2!)^2 + (3!)^2 +...+
 *          (n!)^2.
 * https://oeis.org/A061062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A061062 – Sum of squared factorials
   Formula: a(n) = sum_{k=0..n} (k!)^2
   Recurrence: a(0)=1, a(1)=2,
                a(n) = (n^2+1)*a(n-1) - n^2*a(n-2)   (n≥2)
   The implementation memoises the values in a static array and
   expands it only when a larger index is requested.  All arithmetic
   is performed with the type seq_t, so an arbitrary‑precision build
   automatically provides exact results.  The routine is O(n) for a
   single call but works correctly even when calls arrive in arbitrary
   order because previously computed values are cached. */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;     /* array of a(i) values */
static idx_t  cache_alloc = 0;  /* allocated size of the cache   */
static idx_t  cache_len   = 0;  /* number of entries that are filled */

/* Ensure the cache is allocated with at least 'min_cap' slots. */
static void ensure_cache_size(idx_t min_cap)
{
    if (cache_alloc >= min_cap)
        return;

    /* Allocate at least min_cap slots, doubling until enough */
    idx_t new_cap = cache_alloc ? cache_alloc << 1 : 2;
    while (new_cap < min_cap)
        new_cap <<= 1;

    cache = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
    cache_alloc = new_cap;
}

/* Initialise the cache with the first two terms */
static void init_cache(void)
{
    if (cache_len != 2) {
        ensure_cache_size(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache_len = 2;
    }
}

/* Compute a(n).  The function is safe for any n ≥ 0 and keeps
   previously computed values in the cache. */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)2;

    /* Set up the cache if this is the first call. */
    init_cache();

    /* Expand the cache if necessary. */
    if (cache_len <= n) {
        /* Ensure allocated space. */
        ensure_cache_size((idx_t)(n + 1));
        /* Compute terms from the current length up to n. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t i_val = (seq_t)i;
            seq_t i_sq  = i_val * i_val;            /* i²  */
            seq_t term  = (i_sq + (seq_t)1) * cache[i - 1]; /* (i²+1)*a(i-1) */
            term = term - (i_sq * cache[i - 2]);     /* ... - i²*a(i-2) */
            cache[i] = term;
        }
        /* Record that the cache now contains indices 0..n. */
        cache_len = n + 1;
    }

    return cache[n];
}
