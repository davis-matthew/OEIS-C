/*
 * A042054  Numerators of continued fraction convergents to sqrt(551).
 * https://oeis.org/A042054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = 16760*a(n-6) - a(n-12), a(0)=23, a(1)=47, a(2)=399,
 *            a(3)=446, a(4)=3967, a(5)=8380, a(6)=389447, a(7)=787274,
 *            a(8)=6687639, a(9)=7474913, a(10)=66486943, a(11)=140448799
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t size = 0;      /* number of valid entries in cache */
    static idx_t cap = 0;       /* allocated capacity in entries */

    /* initialise the cache with the first 12 terms */
    if (!cache)
    {
        cap = 64;                     /* arbitrary over‑allocation */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* out‑of‑memory – give up */

        cache[0]  = (seq_t)23;
        cache[1]  = (seq_t)47;
        cache[2]  = (seq_t)399;
        cache[3]  = (seq_t)446;
        cache[4]  = (seq_t)3967;
        cache[5]  = (seq_t)8380;
        cache[6]  = (seq_t)389447;
        cache[7]  = (seq_t)787274;
        cache[8]  = (seq_t)6687639;
        cache[9]  = (seq_t)7474913;
        cache[10] = (seq_t)66486943;
        cache[11] = (seq_t)140448799;
        size     = 12;
    }

    /* return cached value if already computed */
    if (n < size)
        return cache[(size_t)n];

    /* grow the cache if necessary */
    if ((idx_t)(n + 1) > cap)
    {
        size_t new_cap = cap * 2;
        while (new_cap < (size_t)(n + 1))
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)   /* allocation failed – return last known value */
            return cache[(size_t)(size - 1)];
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = size; k <= n; ++k)
    {
        seq_t val = cache[(size_t)(k - 6)] * (seq_t)16760
                  - cache[(size_t)(k - 12)];
        cache[(size_t)k] = val;
    }
    size = n + 1;

    return cache[(size_t)n];
}
