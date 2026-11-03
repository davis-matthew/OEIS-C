/*
 * A208079  Number of 4 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 0 and 0 1 0 vertically.
 * https://oeis.org/A208079
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) + 5*a(n-2) - a(n-3) for n≥4 a(1) = 9, a(2) = 81,
 *            a(3) = 441
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 201 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Local cache for memoisation */
    static seq_t *cache = NULL;   /* array holding already computed values */
    static size_t  cap   = 0;     /* allocated capacity (in indices)     */
    static idx_t   built = 0;     /* highest index computed so far       */

    /* initialise cache on the first call */
    if (cache == NULL) {
        cap   = 4;                 /* indices 0 .. 3 (index 0 unused)    */
        cache = mem_new(seq_t, cap);
        if (cache == NULL) abort(); /* allocation failure – abort program */

        /* initial terms of the sequence (OEIS offset = 1) */
        cache[0] = 0;              /* dummy, unused */
        cache[1] = 9;
        cache[2] = 81;
        cache[3] = 441;
        built   = 3;
    }

    /* if the requested term is already known, return it directly */
    if (n <= built) return cache[n];

    /* grow the cache if necessary */
    if (n >= cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;      /* double until enough */

        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (tmp == NULL) abort();                       /* allocation failure */

        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = built + 1; i <= n; ++i) {
        cache[i] = 5 * cache[i-1] + 5 * cache[i-2] - cache[i-3];
    }
    built = n;
    return cache[n];
}
