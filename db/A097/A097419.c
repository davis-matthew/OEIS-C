/*
 * A097419  a(1) = 2, a(n) = 2*a(n-1)^2 - a(n-1) for n > 1.
 * https://oeis.org/A097419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 2; a(n) = 2*a(n-1)^2 - a(n-1) for n > 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* 1‑based cache of known terms.  seq_cache[0] is unused. */
static seq_t *seq_cache = NULL;
static idx_t cache_size = 0;   /* highest index that has been computed */

/* Return a(n).  It is memoised so repeated calls are cheap. */
seq_t a(idx_t n)
{
    if (n == 0) {                     /* safety guard – offset is 1 */
        return (seq_t)0;
    }

    /* Grow the cache if we do not yet have a(n). */
    if (n > cache_size) {
        /* New size must have room for indices 0..n. */
        idx_t new_size = n + 1;

        /* Use realloc, then cast to the appropriate pointer type. */
#if defined(__cplusplus)
        seq_t *tmp = reinterpret_cast<seq_t *>(
            realloc(seq_cache, (size_t)new_size * sizeof(seq_t)));
#else
        seq_t *tmp = (seq_t *)realloc(seq_cache,
                                      (size_t)new_size * sizeof(seq_t));
#endif
        if (!tmp) {
            /* Allocation failed – return zero (caller can detect error). */
            return (seq_t)0;
        }
        seq_cache = tmp;

        /* Initialise on first allocation. */
        if (cache_size == 0) {
            seq_cache[0] = (seq_t)0;  /* unused but reserved */
            seq_cache[1] = (seq_t)2;  /* a(1) */
            cache_size = 1;
        }

        /* Compute new terms up to n. */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = seq_cache[i - 1];
            seq_t tmp_val = prev * prev;  /* a(i-1)^2 */
            tmp_val = (tmp_val << 1) - prev; /* 2*a(i-1)^2 - a(i-1) */
            seq_cache[i] = tmp_val;
        }
        cache_size = n;
    }

    return seq_cache[n];
}
