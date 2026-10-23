/*
 * A042150  Numerators of continued fraction convergents to sqrt(600).
 * https://oeis.org/A042150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A042150: Numerators of continued fraction convergents to sqrt(600).
 *
 * Recurrence:
 *   a(0) = 24, a(1) = 49, a(2) = 2376, a(3) = 4801,
 *   a(n) = 98*a(n-2) - a(n-4)   for n >= 4
 *
 * This function caches results so that calls in arbitrary order remain
 * efficient.  It works for any seq_t that supports the standard integer
 * operators and for arbitrary‑precision implementations supplied by the build.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* storage for terms */
    static idx_t  cap     = 0;      /* current allocated size */
    static idx_t  last    = 0;      /* number of terms already computed */

    /* Allocate or grow the cache to hold indices up to n */
    if (n + 1 > (idx_t)cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 4) new_cap = 4;          /* at least space for a(0..3) */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* out of memory (unlikely) */
        cache = tmp;
        cap   = (idx_t)new_cap;
    }

    /* Initialise the first four terms once */
    if (last == 0) {
        cache[0] = (seq_t)24;
        cache[1] = (seq_t)49;
        cache[2] = (seq_t)2376;
        cache[3] = (seq_t)4801;
        last    = 4;
    }

    /* If the requested index is within the already computed range, return it */
    if (n < last) return cache[n];

    /* Compute missing terms up to n (recurrence uses a(n‑2) and a(n‑4)) */
    for (idx_t i = last; i <= n; ++i) {
        /* indices 0..3 are already set, so skip them in the loop */
        if (i >= 4) {
            cache[i] = (seq_t)98 * cache[i-2] - cache[i-4];
        }
    }

    last = n + 1;           /* now we have computed terms 0..n inclusive */
    return cache[n];
}
