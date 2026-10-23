/*
 * A031982  a(n) = 1(01)^(2*n+1).
 * https://oeis.org/A031982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=101, a(1)=1010101, a(n)=10001*a(n-1)-10000*a(n-2) for n≥2
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache       = NULL;   /* array of computed terms */
static size_t cap          = 0;     /* allocated capacity (in terms of indices +1) */
static idx_t  cached_max   = (idx_t)-1;   /* highest index computed (-(1) means empty) */

/* Expand the cache so that indices up to 'required' can be accessed. */
static void ensure_capacity(idx_t required)
{
    if (cap > required)
        return;
    size_t new_cap = cap ? cap << 1 : 2;
    while (new_cap <= required)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_arr)
        abort();                      /* allocation failure */
    if (cache) {
        memcpy(new_arr, cache, cap * sizeof(seq_t));
        free(cache);
    }
    cache = new_arr;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Handle the two trivial base cases directly. */
    if (n == 0)
        return (seq_t)101;
    if (n == 1)
        return (seq_t)1010101;

    /* If the requested term is already cached, return it. */
    if (cached_max != (idx_t)-1 && n <= cached_max)
        return cache[n];

    /* Make sure the cache can hold the requested index. */
    ensure_capacity(n);

    /* If this is the very first call, initialise the two base values. */
    if (cached_max == (idx_t)-1) {
        cache[0] = (seq_t)101;
        cache[1] = (seq_t)1010101;
        cached_max = 1;
    }

    /* Compute all terms up to n using the recurrence. */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = (seq_t)10001 * cache[i - 1] - (seq_t)10000 * cache[i - 2];
    }
    cached_max = n;

    return cache[n];
}
