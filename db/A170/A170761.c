/*
 * A170761  Expansion of g.f.: (1+x)/(1-41*x).
 * https://oeis.org/A170761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 0, otherwise a(n) = 42 * 41^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 42, a(n) = 41 * a(n-1) for n >= 2 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

static seq_t *cache = NULL;     /* cached values of the sequence */
static idx_t  cached_len = 0;   /* number of computed entries (valid indices are 0 .. cached_len-1) */

/* Ensure the cache can hold at least 'needed' entries (indices 0..needed-1). */
static void ensure_cache_size(idx_t needed)
{
    if (cached_len >= needed)
        return;

    idx_t cap = (idx_t)cached_len;
    if (cap == 0) cap = 1;
    while (cap < needed)
        cap = cap * 2;

    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)cap * sizeof(seq_t));
    if (!new_cache && cap > 0)  /* allocation failed */
        abort();               /* terminate program on allocation failure */

    /* Compute new entries from cached_len to needed-1 */
    if (cached_len == 0) {
        /* base case a(0) = 1 */
        new_cache[0] = (seq_t)1;
        cached_len = 1;
    }

    for (idx_t i = cached_len; i < needed; ++i) {
        if (i == 1) {
            new_cache[1] = (seq_t)42;          /* a(1) = 42 */
        } else {
            new_cache[i] = (seq_t)41 * new_cache[i - 1];
        }
    }

    cache = new_cache;
    cached_len = needed;
}

seq_t a(idx_t n)
{
    ensure_cache_size(n + 1);
    return cache[n];
}
