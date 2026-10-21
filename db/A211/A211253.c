/*
 * A211253  Number of (n+1) X (n+1) -6..6 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or two distinct values.
 * https://oeis.org/A211253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) - a(n-3),  a(1)=21, a(2)=29, a(3)=41
 *  (OEIS A211253)
 *  The function memoises results in a static array so that multiple
 *  calls in any order are handled efficiently.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for previously computed terms.
 * cache[0] holds a(1), cache[1] holds a(2), etc. */
static seq_t *cache = NULL;
static size_t cache_capacity = 0;   /* number of allocated slots */
static size_t cache_size = 0;       /* number of cached terms (>=1) */

/* Ensure the cache can hold at least `need` terms.  The cache is
 * grown geometrically to keep reallocations infrequent. */
static void ensure_cache(size_t need)
{
    if (cache_capacity >= need)
        return;
    size_t new_cap = (cache_capacity == 0 ? 3 : cache_capacity * 2);
    if (new_cap < need)
        new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);          /* fatal error: out of memory */
    cache = tmp;
    cache_capacity = new_cap;
}

/* Return the n‑th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)                      /* offset is 1; undefined lower bound */
        return (seq_t)0;

    /* initialise cache with the first three terms if empty */
    if (cache_size == 0) {
        ensure_cache(3);
        cache[0] = (seq_t)21;
        cache[1] = (seq_t)29;
        cache[2] = (seq_t)41;
        cache_size = 3;
    }

    /* If requested term already cached, return it */
    if ((size_t)n <= cache_size)
        return cache[(size_t)n - 1];

    /* Compute and cache terms up to n */
    ensure_cache((size_t)n);
    for (idx_t k = (idx_t)cache_size; k < n; ++k) {
        size_t pos = (size_t)k;          /* zero‑based index for a(k+1) */
        seq_t val;
        if (pos == 0)
            val = (seq_t)21;             /* a(1) */
        else if (pos == 1)
            val = (seq_t)29;             /* a(2) */
        else if (pos == 2)
            val = (seq_t)41;             /* a(3) */
        else
            val = ((seq_t)2 * cache[pos - 1]) - cache[pos - 3];
        cache[pos] = val;
    }
    cache_size = (size_t)n;
    return cache[(size_t)n - 1];
}
