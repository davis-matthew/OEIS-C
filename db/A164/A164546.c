/*
 * A164546  a(n) = 8*a(n-1) - 8*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A164546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*a(n-1) - 8*a(n-2) for n > 1; a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate a growable cache of previously computed values.  */
    static seq_t *cache = NULL;
    static idx_t  cap   = 0;   /* current allocated size (in seq_t elements) */
    static idx_t  used  = 0;   /* number of values we have computed so far */

    /* Initialise the cache with the first two terms. */
    if (cap == 0) {
        cap   = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) return (seq_t)0;          /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        used   = 2;
    }

    /* Grow the cache if we are asked for a term beyond the current capacity. */
    if (n >= cap) {
        idx_t newcap = cap;
        while (newcap <= n) newcap <<= 1;     /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * newcap);
        if (!tmp) return (seq_t)0;            /* realloc failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing terms up to index n. */
    for (idx_t i = used; i <= n; ++i) {
        if (i == 0 || i == 1) {
            /* already initialized */
        } else {
            cache[i] = (seq_t)8 * cache[i - 1] - (seq_t)8 * cache[i - 2];
        }
    }
    used = n + 1;

    return cache[n];
}
