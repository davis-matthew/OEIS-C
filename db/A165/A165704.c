/*
 * A165704  a(n) = a([n/2]) + a([n/3]) + a([n/5]).
 * https://oeis.org/A165704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a([n/2]) + a([n/3]) + a([n/5]), a(0) = 1
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Cached values */
static seq_t *cache          = NULL;   /* dynamic array for computed terms */
static size_t  cache_cap     = 0;      /* current allocated size (>=1) */
static idx_t   max_computed  = (idx_t)-1; /* largest n for which a(n) is known */

/* Ensure the cache can hold indices 0..n.  Newly allocated slots are
 * zeroed so uncomputed entries are 0. */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;

    size_t new_cap = need * 2;
    if (new_cap < 8)          /* minimal growth */
        new_cap = 8;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();               /* out of memory */

    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = (seq_t)0;

    cache      = tmp;
    cache_cap  = new_cap;
}

/* Compute a(i) for all i in (max_computed, n] inclusive.  The
 * recurrence refers only to smaller indices, so a forward loop suffices. */
static void compute_to(idx_t n)
{
    if (max_computed != (idx_t)-1 && n <= max_computed)
        return;

    idx_t start = (max_computed == (idx_t)-1) ? 0 : max_computed + 1;
    for (idx_t i = start; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;   /* base case */
        }
        else
        {
            cache[i] = cache[i/2] + cache[i/3] + cache[i/5];
        }
    }
    max_computed = n;
}

/* Public sequence function (external linkage). */
seq_t a(idx_t n)
{
    ensure_cache(n);
    compute_to(n);
    return cache[n];
}
