/*
 * A147652  Expansion of 1/(1 - x^4 - x^5 - x^6 + x^10).
 * https://oeis.org/A147652
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-4) + a(n-5) + a(n-6) - a(n-10) , n > 9 with a(0)=1,
 *            a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=1, a(6)=1, a(7)=0, a(8)=1,
 *            a(9)=2
 * Range:     u8 n <= 35, u16 n <= 63, u32 n <= 120, u64 n <= 233,
 *            u128 n <= 460, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    static seq_t *cache = NULL;  /* array of computed terms   */
    static size_t cap   = 0;     /* current allocated size    */
    static idx_t  max_n = 0;     /* largest index already computed */

    /* first call – initialise cache with base values 0..9 */
    if (!cache)
    {
        cap = 10;                                  /* need indices 0..9 */
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return 0;                      /* allocation failed */

        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 1;
        cache[5] = 1;
        cache[6] = 1;
        cache[7] = 0;
        cache[8] = 1;
        cache[9] = 2;
        max_n = 9;
    }

    /* if the requested index is already known, return it */
    if (n <= max_n)
        return cache[n];

    /* grow the cache so that it can hold up to index n  */
    if ((size_t)n >= cap)
    {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until big enough */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;         /* realloc failed */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i)
    {
        seq_t val = 0;
        if (i >= 4) val += cache[i - 4];
        if (i >= 5) val += cache[i - 5];
        if (i >= 6) val += cache[i - 6];
        if (i >= 10) val -= cache[i - 10];
        cache[i] = val;
    }

    max_n = n;
    return cache[n];
}
