/*
 * A158535  a(n) = Hermite(n,11).
 * https://oeis.org/A158535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=22, a(n)=22*a(n-1)-2*(n-1)*a(n-2)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 6, i64 n <= 14, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, NULL */

seq_t a(idx_t n)
{
    /* static cache for previously computed terms */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cap  = 0;       /* allocated capacity (in elements) */
    static idx_t last  = 1;       /* index of the last filled term */

    /* initialise cache with the first two terms */
    if (cache == NULL) {
        /* ensure capacity for indices 0 and 1 */
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)22;
    }

    /* if the requested index is already cached, return it */
    if (n <= last) {
        return cache[n];
    }

    /* grow capacity as needed */
    while (cap <= n) {
        size_t new_cap = cap == 0 ? 2 : cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing entries using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t term = (seq_t)22 * cache[i - 1]
                     - (seq_t)2 * ((seq_t)(i - 1) * cache[i - 2]);
        cache[i] = term;
    }

    last = n;
    return cache[n];
}
