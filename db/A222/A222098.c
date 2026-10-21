/*
 * A222098  Number of n X 2 0..5 arrays with entries increasing mod 6 by 0, 1,
 *          2 or 3 rightwards and downwards, starting with upper left zero.
 * https://oeis.org/A222098
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  
 * Formula: a(n) = 13*a(n-1) - 17*a(n-2)
 * Initial values: a(1) = 4,  a(2) = 46
 */
seq_t a(idx_t n)
{
    /* Cache of already computed values.  cache[i] = a(i+1). */
    static seq_t *cache   = NULL;  /* pointer to stored terms */
    static idx_t  len     = 0;     /* number of terms that have been computed */
    static idx_t  alloc   = 0;     /* allocated size of cache */

    if (n < 1) return 0;           /* undefined for n = 0, but safe */

    /* Initialisation: store a(1) and a(2). */
    if (cache == NULL) {
        alloc = 2;
        cache = (seq_t*)malloc(sizeof(seq_t) * alloc);
        if (!cache) return 0;     /* allocation failed – return 0 */
        cache[0] = (seq_t)4;  /* a(1) */
        cache[1] = (seq_t)46; /* a(2) */
        len = 2;
    }

    /* If the requested n is not yet available, grow the cache as needed
       and then compute subsequent terms using the recurrence. */
    if (n > len) {
        /* Ensure the array has room for at least n terms. */
        if (alloc < n) {
            idx_t new_alloc = alloc;
            while (new_alloc < n) {
                new_alloc <<= 1;               /* double the size */
                if (new_alloc < alloc)         /* overflow guard */
                    new_alloc = n;
            }
            seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_alloc);
            if (!tmp) return 0;                /* realloc failed */
            cache = tmp;
            alloc = new_alloc;
        }

        /* Compute new terms from len up to n-1 (zero‑based indices). */
        for (idx_t i = len; i < n; ++i) {
            /* cache[i] = a(i+1) = 13*a(i) - 17*a(i-1) */
            cache[i] = (seq_t)13 * cache[i-1] - (seq_t)17 * cache[i-2];
        }
        len = n;   /* now we have computed up to a(n) */
    }

    return cache[n-1];
}
