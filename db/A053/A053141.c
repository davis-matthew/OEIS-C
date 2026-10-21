/*
 * A053141  a(0)=0, a(1)=2 then a(n) = a(n-2) + 2*sqrt(8*a(n-1)^2 + 8*a(n-1) +
 *          1).
 * https://oeis.org/A053141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=2, a(n)=6*a(n-1)-a(n-2)+2 (n≥2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of a[0..cap-1] */
    static idx_t  cap   = 0;      /* allocated size */
    static idx_t  max_n = 0;      /* highest index computed so far */

    /* grow the cache if required */
    if (n >= cap) {
        idx_t newcap = (cap == 0 ? 4 : cap << 1);
        while (newcap <= n) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* initialise base values on first use */
    if (max_n == 0) {
        cache[0] = (seq_t)0;
        if (cap > 1) cache[1] = (seq_t)2;
        max_n = (cap > 1 ? 1 : 0);
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        seq_t t = 6 * cache[i - 1];
        t -= cache[i - 2];
        t += (seq_t)2;
        cache[i] = t;
    }
    max_n = n;

    return cache[n];
}
