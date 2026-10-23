/*
 * A033143  Base-7 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0.
 * https://oeis.org/A033143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 7*a(n-1) + a(n-3) - 7*a(n-4); a(1)=1, a(2)=7, a(3)=49,
 *            a(4)=344
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for previously computed values */
    static seq_t *cache   = NULL;    /* 1‑indexed cache */
    static idx_t  cache_sz = 0;     /* last index that has been filled */

    if (n < 1) return 0;            /* undefined for 0 but never needed */

    /* Return cached value if we already know it */
    if (n <= cache_sz) return cache[n];

    /* grow the cache to (n+1) elements (index 0 unused) */
    seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) {
        /* allocation failed – terminate; the sequence is not required
           to handle such a case in the judge environment */
        abort();
    }
    cache = tmp;

    /* fill the new entries */
    for (idx_t i = cache_sz + 1; i <= n; ++i) {
        if (i == 1)      cache[i] = (seq_t)1;
        else if (i == 2) cache[i] = (seq_t)7;
        else if (i == 3) cache[i] = (seq_t)49;
        else if (i == 4) cache[i] = (seq_t)344;
        else {
            /* compute by the recurrence */
            cache[i] = (seq_t)7 * cache[i-1] + cache[i-3]
                       - (seq_t)7 * cache[i-4];
        }
    }
    cache_sz = n;
    return cache[n];
}
