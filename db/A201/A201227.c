/*
 * A201227  a(n) = (A201225(n))^3 - (A201226(n))^2.
 * https://oeis.org/A201227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 19*a(n-1) - 19*a(n-2) + a(n-3) with a(1)=219375,
 *            a(2)=4566375, a(3)=82569375 (a(n) = (A201225(n))^3 -
 *            (A201226(n))^2)
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* static cache of computed terms: 1‑based indexing */
    static seq_t *cache = NULL;      /* array of seq_t values */
    static size_t  cap   = 0;        /* current capacity (number of slots) */
    static idx_t   top   = 0;        /* highest n computed so far */

    if (n < 1)                 /* undefined for n < 1 – return 0 */
        return (seq_t)0;

    /* allocate or grow the cache to hold index n */
    if (cache == NULL) {
        /* first allocation: at least up to 3 to store the base cases */
        size_t init_cap = (size_t)(n > 3 ? n : 3) + 1;
        cache = (seq_t *)calloc(init_cap, sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* no memory – bail out */
        cap = init_cap;
        /* initialise known terms */
        cache[1] = (seq_t)219375;
        cache[2] = (seq_t)4566375;
        cache[3] = (seq_t)82569375;
        top = 3;
    } else if ((size_t)n >= cap) {
        /* grow the array just enough for the new index */
        size_t new_cap = (size_t)(n) + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* no memory – bail out */
        /* zero‑initialize the new part */
        if (new_cap > cap)
            for (size_t i = cap; i < new_cap; ++i)
                tmp[i] = 0;
        cache = tmp;
        cap = new_cap;
    }

    /* if the value is already known, return it */
    if (n <= top)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = top + 1; i <= n; ++i) {
        cache[i] = (seq_t)19 * cache[i - 1] -
                   (seq_t)19 * cache[i - 2] +
                   cache[i - 3];
    }

    top = n;
    return cache[n];
}
