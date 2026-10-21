/*
 * A059162  A hierarchical sequence (S(W'2{3}*c) - see A059126).
 * https://oeis.org/A059162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12 * (n + 1) * 2^n (equivalently: a(0) = 12, a(1) = 48,
 *            a(n) = 4 * a(n-1) - 4 * a(n-2) for n > 1)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 23, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Assuming seq_t and idx_t are already defined in the build environment */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *cache = NULL;   /* array of computed a(k) */
    static idx_t  cache_used   = 0;  /* number of entries actually computed */
    static size_t cache_cap    = 0;   /* allocated capacity of the array */

    /* Ensure the cache has at least the first two values pre‑computed. */
    if (cache_cap == 0) {
        cache_cap = 4;                     /* start with four slots */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;       /* allocation failed – return 0 */
        cache[0] = (seq_t)12;              /* a(0) */
        cache[1] = (seq_t)48;              /* a(1) */
        cache_used = 2;
    }

    /* Grow the cache if we are asked for an index that is not yet present. */
    while (cache_used <= n) {
        /* Compute a(new_index) by the recurrence: a = 4*a(n-1) - 4*a(n-2) */
        idx_t  i   = cache_used;                 /* new index to compute */
        seq_t  val = 4 * cache[i-1] - 4 * cache[i-2];

        /* Ensure there is space for the new value. */
        if (cache_used == cache_cap) {
            cache_cap <<= 1;                      /* double the capacity */
            seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* reallocation failed */
            cache = tmp;
        }

        cache[cache_used++] = val;               /* store the new value */
    }

    return cache[n];
}
