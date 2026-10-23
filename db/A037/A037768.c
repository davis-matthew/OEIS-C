/*
 * A037768  Base 7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 3,0,2,1.
 * https://oeis.org/A037768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 3, a(2) = 21, a(3) = 149, a(4) = 1044, a(5) = 7311 a(n) =
 *            7*a(n-1) + a(n-4) - 7*a(n-5) for n ≥ 6 a(n) is defined for n ≥ 1
 *            (offset 1)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)   /* undefined offset – return 0 */
        return (seq_t)0;

    /* Convert idx_t to size_t for array indexing.  */
    size_t idx = (size_t)n;

    /* Static cache for memoisation.  */
    static seq_t *cache  = NULL;
    static size_t  cap   = 0;       /* allocated length (including index 0)   */
    static size_t  last  = 0;       /* highest index that has been computed   */
    static bool    init  = false;   /* whether the first five terms are stored */

    /* Ensure the cache is large enough.  */
    if (idx + 1 > cap) {
        size_t newcap = idx + 1;
        if (newcap < 8) newcap = 8;           /* give some spare room */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)  /* allocation failure – fall back to zero */
            return (seq_t)0;
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise the five known terms once.  */
    if (!init) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)21;
        cache[3] = (seq_t)149;
        cache[4] = (seq_t)1044;
        cache[5] = (seq_t)7311;
        last    = 5;
        init    = true;
    }

    /* If the requested term is within the cached range, return it directly. */
    if (idx <= last)
        return cache[idx];

    /* Compute any missing terms up to idx using the recurrence. */
    for (size_t i = last + 1; i <= idx; ++i) {
        seq_t term =
            (seq_t)7 * cache[i - 1]  /* 7*a(n-1) */
            + cache[i - 4]          /* + a(n-4) */
            - (seq_t)7 * cache[i - 5]; /* - 7*a(n-5) */
        cache[i] = term;
    }
    last = idx;

    return cache[idx];
}
