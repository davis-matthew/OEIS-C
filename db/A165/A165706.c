/*
 * A165706  a(0) = 1, a(n) = a([n/2]) + a([n/5]) for n > 1.
 * https://oeis.org/A165706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, * a(n) = a(floor(n/2)) + a(floor(n/5)) for n > 0
 *            (recurrence)
 * Range:     u8 n <= 3199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache indexed by 0 .. n */
static size_t cache_cap = 0;       /* currently allocated size */

/* Grow the cache so that it can store entries up to index n */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;

#ifdef __cplusplus
    /* C++: use new/delete and copy existing elements */
    seq_t *tmp = new seq_t[needed];
    for (size_t i = 0; i < cache_cap; ++i)
        tmp[i] = seq_cache[i];
    if (seq_cache)
        delete[] seq_cache;
    /* initialise new entries to zero (default constructor or zero) */
    for (size_t i = cache_cap; i < needed; ++i)
        tmp[i] = seq_t();
    seq_cache = tmp;
    cache_cap = needed;
#else
    /* C: use realloc, initialise added entries to 0 */
    seq_t *tmp = (seq_t *)realloc(seq_cache, needed * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);
    for (size_t i = cache_cap; i < needed; ++i)
        tmp[i] = 0;
    seq_cache = tmp;
    cache_cap = needed;
#endif
}

/* Recursive computation with memoisation */
static seq_t seq_compute(idx_t n)
{
    ensure_capacity(n);

    /* base case a(0) */
    if (n == 0) {
        if (seq_cache[0] == 0)
            seq_cache[0] = 1;
        return seq_cache[0];
    }

    /* use cached value if ready */
    if (seq_cache[n] != 0)
        return seq_cache[n];

    seq_t left  = seq_compute(n / 2);
    seq_t right = seq_compute(n / 5);
    seq_cache[n] = left + right;    /* store and return */
    return seq_cache[n];
}

/* Public interface ----------------------------------------------------- */
seq_t a(idx_t n)
{
    return seq_compute(n);
}
