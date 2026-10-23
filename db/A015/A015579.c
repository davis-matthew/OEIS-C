/*
 * A015579  Expansion of g.f. x/(1 - 9*x - 2*x^2).
 * https://oeis.org/A015579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) + 2*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return the n-th term of OEIS sequence A015579.  The function
   memoises calculated values in a static array so that calls in
   any order are answered in O(1) after a linear‑time fill.  */
seq_t a(idx_t n)
{
    /* Cache of generated terms, from a(0) up to a(cached_n).  */
    static seq_t *cache = NULL;
    static idx_t cached_n = 0;   /* highest index already stored */

    /* ----- Initialization or expansion of the cache ----- */
    if (cache == NULL)                     /* first call */
    {
        /* Allocate space for 0..n inclusive.  */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)calloc(sz, sizeof(seq_t));
        if (cache == NULL)                /* alloc failure → zero */
            return (seq_t)0;

        /* Base cases.  */
        cache[0] = (seq_t)0;
        if (n >= 1)
            cache[1] = (seq_t)1;

        /* Compute remaining terms.  */
        for (idx_t i = 2; i <= n; ++i)
            cache[i] = 9 * cache[i - 1] + 2 * cache[i - 2];

        cached_n = n;
        return cache[n];
    }

    /* If requested index is already cached, just return it. */
    if (n <= cached_n)
        return cache[n];

    /* ----- Expand cache to include index n ----- */
    size_t new_sz = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
    if (tmp == NULL)                      /* realloc failed → zero */
        return (seq_t)0;
    cache = tmp;

    /* Compute only the newly required terms.  */
    for (idx_t i = cached_n + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[1] = (seq_t)1;          /* base case for i==1 */
        else
            cache[i] = 9 * cache[i - 1] + 2 * cache[i - 2];
    }

    cached_n = n;
    return cache[n];
}
