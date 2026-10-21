/*
 * A095982  a(n) = a(n-1) + a(n-2) + a(n-4) with a(0) = 2, a(1) = 3, a(2) = 6,
 *          a(3) = 9.
 * https://oeis.org/A095982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 38, u64 n <= 77, u128 n <= 156,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + a(n-4),
   a(0) = 2, a(1) = 3, a(2) = 6, a(3) = 9 */

/* Computes the nth term of OEIS sequence A095982 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;      /* allocated capacity in elements */
    static idx_t computed_max = (idx_t)-1;   /* highest index computed */

    /* Expand cache if necessary so that it can hold index n */
    if (cache_cap <= n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 4;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_ptr) { /* allocation failure – abort */
            return 0;
        }
        cache = new_ptr;
        cache_cap = new_cap;
    }

    /* Initialise base terms on first use */
    if (computed_max == (idx_t)-1) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)6;
        cache[3] = (seq_t)9;
        computed_max = 3;
    }

    /* Compute missing terms up to n */
    while (computed_max < n) {
        ++computed_max;
        if (computed_max >= 4) {
            cache[computed_max] = cache[computed_max-1]
                                 + cache[computed_max-2]
                                 + cache[computed_max-4];
        }
        /* else base values already set */
    }

    return cache[n];
}
