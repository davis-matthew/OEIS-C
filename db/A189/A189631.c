/*
 * A189631  Partial sums of A189628.
 * https://oeis.org/A189631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Partial sums of A189628
   Recurrence:  a(n) = floor(n/3) + a(floor((n+1)/3)) - a(floor(n/3)),
   a(0) = 0.  The function returns a(n) for n >= 1.  The implementation
   memoises results in a static cache so that arbitrary order of calls
   is supported. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Static cache for memoisation */
static seq_t *cache = NULL;   /* array of computed values */
static size_t cache_cap = 0;  /* allocated number of elements */
static size_t cache_next = 1; /* next index that still needs to be computed */

/* Return the n‑th term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1;  a(0) is not used, but return 0 */
        return (seq_t)0;

    /* Ensure cache is large enough to hold index n. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = NULL;

        if (cache == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        }
        if (tmp == NULL)   /* allocation failed */ 
            return (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise base value if this is the first call. */
    if (cache_next == 1 && cache_cap > 0) {
        cache[0] = (seq_t)0;  /* a(0) = 0 for recurrence use */
    }

    /* Compute all missing entries up to n. */
    for (idx_t i = cache_next; i <= n; ++i) {
        /* floor(i/3) */
        seq_t f_i_div3 = (seq_t)(i / 3);
        /* a(floor((i+1)/3)) */
        seq_t a_floor_ip1_3 = cache[(i + 1) / 3];
        /* a(floor(i/3)) */
        seq_t a_floor_i_3 = cache[i / 3];
        cache[i] = f_i_div3 + a_floor_ip1_3 - a_floor_i_3;
    }
    cache_next = n + 1;

    return cache[n];
}
