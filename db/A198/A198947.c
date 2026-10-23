/*
 * A198947  x values in the solution to 11*x^2 - 10 = y^2.
 * https://oeis.org/A198947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n+4) = 20*a(n+2) - a(n) with a(1)=1, a(2)=7, a(3)=13, a(4)=139
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache holding values a[1..last]  */
    static seq_t *cache = NULL;   /* cache[0] unused   */
    static idx_t last  = 0;       /* largest index known */
    static size_t cap   = 0;      /* allocated size     */

    /* initialise cache with the first four terms */
    if (cache == NULL) {
        cap = 4;
        cache = (seq_t *)malloc((cap + 1) * sizeof(seq_t));
        if (cache == NULL) exit(EXIT_FAILURE); /* allocation failed */
        cache[1] = 1;
        cache[2] = 7;
        cache[3] = 13;
        cache[4] = 139;
        last = 4;
    }

    /* if the requested term is already known, return it */
    if (n <= last) {
        return cache[n];
    }

    /* grow the cache if necessary */
    if ((size_t)n > cap) {
        size_t new_cap = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = last + 1; i <= n; ++i) {
        /* multiplication and subtraction are performed in seq_t type  */
        cache[i] = 20 * cache[i - 2] - cache[i - 4];
    }
    last = n;
    return cache[n];
}
