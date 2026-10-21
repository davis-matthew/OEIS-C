/*
 * A223434  Generalized Petersen graph (8,2) coloring a rectangular array:
 *          number of n X 2 0..15 arrays where 0..15 label nodes of a graph
 *          with edges 0,1 0,8 8,14 8,10 1,2 1,9 9,15 9,11 2,3 2,10 10,12 3,4
 *          3,11 11,13 4,5 4,12 12,14 5,6 5,13 13,15 6,7 6,14 7,0 7,15 and
 *          every array movement to a horizontal or v...
 * https://oeis.org/A223434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 8*a(n-1) - 11*a(n-2) - 16*a(n-3)
   Initial values: a(1)=48, a(2)=256, a(3)=1376 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle offset: values for n < 1 are ignored */
    if (n == 0) return 0;

    /* Static cache for memoized values */
    static seq_t *cache = NULL;
    static size_t cap = 0;   /* current allocation size */
    static size_t len = 0;   /* number of terms stored (a(1)..a(len)) */

    /* Initialize cache with first three terms on first call */
    if (len == 0) {
        cap = 3;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                     /* allocation failed */
        cache[0] = (seq_t)48;   /* a(1) */
        cache[1] = (seq_t)256;  /* a(2) */
        cache[2] = (seq_t)1376; /* a(3) */
        len = 3;
    }

    /* Grow cache if necessary */
    if ((idx_t)len < n) {
        size_t new_cap = cap;
        while (new_cap < (size_t)n) {
            new_cap = new_cap ? new_cap * 2 : 2;
        }
        if (new_cap > cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;                    /* allocation failed */
            cache = tmp;
            cap = new_cap;
        }
        /* Compute missing terms using recurrence */
        for (idx_t k = (idx_t)len + 1; k <= n; ++k) {
            /* Indices in cache are zero‑based: a(i) -> cache[i-1] */
            seq_t term = (seq_t)8 * cache[(size_t)k - 2]
                        - (seq_t)11 * cache[(size_t)k - 3]
                        - (seq_t)16 * cache[(size_t)k - 4];
            cache[(size_t)k - 1] = term;
        }
        len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
