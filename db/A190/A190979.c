/*
 * A190979  a(n) = 9*a(n-1) - 2*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 2*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* A simple memoized linear‑time implementation for the recurrence.  The
 * cache grows as needed and works for any seq_t type (including an arbitrary‑
 * precision C++ class).  The function is thread‑unsafe but complies with the
 * project constraints. */
static seq_t *seq_cache   = NULL;   /* cache array */
static idx_t  cache_cap   = 0;      /* capacity of cache array */
static idx_t  cache_len   = 0;      /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* ensure capacity for index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap << 1 : 8;
        if (new_cap < n + 1)
            new_cap = n + 1;
        /* cast is required for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* out of memory */
        seq_cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first two terms if needed */
    if (cache_len == 0) {
        seq_cache[0] = 0;
        if (cache_cap > 1)
            seq_cache[1] = 1;
        cache_len = 2;
    }

    /* compute missing terms up to n */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_cache[i] = 9 * seq_cache[i - 1] - 2 * seq_cache[i - 2];
        }
        cache_len = n + 1;
    }

    return seq_cache[n];
}
