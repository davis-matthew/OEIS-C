/*
 * A097477  a(n) = number of earlier terms greater than a(floor(n/2)); a(1)=1.
 * https://oeis.org/A097477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of earlier terms greater than a(floor(n/2)), a(1)=1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache      = NULL;   /* array indexed from 1 */
    static idx_t  cache_size = 0;      /* largest index currently guaranteed in cache */
    static size_t cache_cap  = 0;      /* number of elements allocated (including unused 0) */

    if (n == 0)  /* undefined, but guard against under‑flow in callers */
        return (seq_t)0;

    /* Expand cache to contain at least index n */
    if (n > cache_size) {
        size_t new_cap = (size_t)n + 1;          /* indices 0 … n */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (cache == NULL) exit(1);         /* allocation failure */
            cache_cap = new_cap;
        } else if (new_cap > cache_cap) {
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) exit(1);           /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i == 1) {                /* base case */
                cache[i] = (seq_t)1;
                continue;
            }

            idx_t       d   = i >> 1;       /* floor(i/2) */
            seq_t       val = cache[d];     /* a(floor(i/2)) */

            idx_t count = 0;
            for (idx_t j = 1; j < i; ++j) {
                if (cache[j] > val) ++count;
            }
            cache[i] = (seq_t)count;
        }
        cache_size = n;
    }

    return cache[n];
}
