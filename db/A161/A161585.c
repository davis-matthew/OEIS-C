/*
 * A161585  The list of the k values in the common solutions to the 2
 *          equations 7*k+1 = A^2, 11*k+1 = B^2.
 * https://oeis.org/A161585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=9, a(3)=720, a(n+3) = 80 * ( a(n+2) - a(n+1) ) +
 *            a(n) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of OEIS A161585 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the off–by‑one index and undefined 0 case */
    if (n == 0) return (seq_t)0;

    /* Static cache: cache[i] = a(i+1)  (0‑based) */
    static seq_t *cache = NULL;
    static size_t   cap   = 0;   /* allocated capacity (in terms)   */
    static size_t   len   = 0;   /* number of terms actually stored */

    /* Initialise the cache with the first three terms */
    if (cache == NULL) {
        cap = 4;                                 /* room for indices 1–3 */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;      /* allocation failed */
        cache[0] = (seq_t)0;    /* a(1) */
        cache[1] = (seq_t)9;    /* a(2) */
        cache[2] = (seq_t)720;  /* a(3) */
        len = 3;
    }

    /* If the requested index is already cached, return it */
    if (n <= len)
        return cache[n - 1];

    /* Expand the cache capacity if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = cap;
        while ((size_t)n > new_cap)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;      /* realloc failed */
        cache = tmp;
        cap = new_cap;
    }

    /* Generate the missing terms using the recurrence */
    for (size_t idx = len; idx < (size_t)n; ++idx) {
        /* idx corresponds to term a(idx+1) */
        seq_t diff = cache[idx - 1] - cache[idx - 2];
        seq_t prod = diff * (seq_t)80;
        cache[idx] = prod + cache[idx - 3];
    }
    len = (size_t)n;
    return cache[n - 1];
}
