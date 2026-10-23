/*
 * A033971  Trajectory of 1 under map n->29n+1 if n odd, n->n/2 if n even.
 * https://oeis.org/A033971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 17, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1;
   a(n+1) = (a(n) is even) ? a(n)/2 : 29*a(n)+1   */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache of computed terms */
static size_t  cap   = 0;     /* allocated size of cache    */
static size_t  last  = 0;     /* index of last computed term */

/* Ensure that cache[n] is valid for all indices up to n */
static void
ensure_cache(idx_t n)
{
    size_t idx = (size_t)n;

    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        size_t new_cap = 1;
        /* Grow until capacity > idx */
        while (idx + 1 > new_cap) new_cap <<= 1;
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) */
        cap   = new_cap;
        last  = 0;
    } else if (idx >= cap) {
        size_t new_cap = cap;
        while (idx >= new_cap) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute missing terms up to idx */
    for (size_t i = last + 1; i <= idx; ++i) {
        seq_t prev = cache[i - 1];
        if ((prev & (seq_t)1) == 0)          /* even */
            cache[i] = prev >> 1;            /* prev / 2 */
        else                                 /* odd  */
            cache[i] = prev * (seq_t)29 + (seq_t)1;
    }
    last = idx;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
