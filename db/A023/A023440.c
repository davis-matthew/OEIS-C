/*
 * A023440  Dying rabbits: a(n) = a(n-1) + a(n-2) - a(n-10).
 * https://oeis.org/A023440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-10) * initial values: a(0)=0,
 *            a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=5, * a(6)=8, a(7)=13,
 *            a(8)=21, a(9)=34
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 48, u64 n <= 94, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache to memoise computed terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;

    /* initial values for indices 0..9 */
    static const seq_t init[10] = {
        0, 1, 1, 2, 3,
        5, 8,13,21,34
    };

    /* for indices below the offset just return the hard‑coded value */
    if (n < 10)
        return init[(size_t)n];

    /* initialise cache on first use (allocate space for the first 10 terms) */
    if (cache == NULL) {
        cap   = 10;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache)
            return 0;  /* out of memory – caller will handle */
        for (size_t i = 0; i < 10; i++)
            cache[i] = init[i];
    }

    /* grow the cache if needed */
    if ((size_t)n >= cap) {
        size_t newcap = ((size_t)n + 1);
        if (newcap < cap * 2)
            newcap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return 0;  /* out of memory – caller will handle */
        cache = tmp;
        cap   = newcap;
    }

    /* compute missing terms using the recurrence */
    for (size_t i = 10; i <= (size_t)n; i++) {
        cache[i] = cache[i-1] + cache[i-2] - cache[i-10];
    }

    return cache[(size_t)n];
}
