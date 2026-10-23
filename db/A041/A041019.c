/*
 * A041019  Denominators of continued fraction convergents to sqrt(13).
 * https://oeis.org/A041019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = A010122(n)*a(n-1)+a(n-2), with a(0)=1, a(-1)=0.
 *            A010122(n) gives the continued‑fraction partial quotients for
 *            sqrt(13), which repeat the pattern 1,1,1,1,6
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 63, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include standard library for memory allocation. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static cache of previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cached_n = (idx_t)-1;   /* indicates no values cached yet */

    /* Initialise cache for n = 0 and n = 1. */
    if (cache == NULL) {
        cache = (seq_t *)calloc((size_t)2, sizeof(seq_t));
        if (cache == NULL) {          /* allocation failure – return 0 */ 
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;          /* a(0) */
        cache[1] = (seq_t)1;          /* a(1) */
        cached_n = 1;
    }

    /* If requested term is already cached, return it immediately. */
    if (n <= cached_n) {
        return cache[n];
    }

    /* Expand the cache to hold terms up to n. */
    seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
    if (new_cache == NULL) {          /* realloc failure – fallback to last value */
        return cache[cached_n];
    }
    cache = new_cache;

    /* Compute missing terms from cached_n + 1 up to n. */
    for (idx_t i = cached_n + 1; i <= n; ++i) {
        /* Determine the i-th continued‑fraction coefficient for sqrt(13). */
        seq_t coeff;
        idx_t rel = (i - 1) % 5;
        coeff = (rel == 4) ? (seq_t)6 : (seq_t)1;

        /* Recurrence: a(i) = coeff * a(i-1) + a(i-2). */
        cache[i] = coeff * cache[i - 1] + cache[i - 2];
    }

    /* Update cached limit and return requested value. */
    cached_n = n;
    return cache[n];
}
