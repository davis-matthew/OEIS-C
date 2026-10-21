/*
 * A087834  a[n] =a[a[a[n-1]]] + a[n -1- a[n-2]].
 * https://oeis.org/A087834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a[a[a[n-1]]] + a[n -1- a[n-2]] (offset 1)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Memoised, bottom‑up implementation.  Works for any unsigned
   16/32/64/128‑bit seq_t or arbitrary‑precision implementation. */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* cache[0] is unused; cache[1] = a(1), cache[2] = a(2) are the only
       required base values.  The cache grows as needed. */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;        /* largest n for which cache[n] is valid */

    if (n < 1)                /* sequence is defined for n >= 1 only */
        return 0;

    /* initialise cache with indices 0,1,2 */
    if (cached_len == 0) {
        cache = (seq_t *)calloc(3, sizeof(seq_t));   /* indices 0..2 */
        if (!cache)
            abort();
        cache[1] = (seq_t)1;      /* a(1) */
        cache[2] = (seq_t)1;      /* a(2) */
        cached_len = 2;
    }

    /* grow cache to include the requested n */
    if (n > cached_len) {
        size_t new_size = (size_t)n + 1;              /* indices 0..n */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            abort();
        cache = tmp;

        /* compute terms from cached_len+1 up to n */
        for (size_t i = (size_t)cached_len + 1; i <= (size_t)n; ++i) {
            /* first part: a[a[a[i-1]]] */
            idx_t v1 = (idx_t)cache[i - 1];    /* a[i-1]          */
            idx_t v2 = (idx_t)cache[v1];       /* a[a[i-1]]       */
            seq_t first = cache[v2];           /* a[a[a[i-1]]]    */

            /* second part: a[i-1 - a[i-2]] */
            idx_t idx2 = (idx_t)((idx_t)i - 1 - (idx_t)cache[i - 2]);   /* n-1-a[n-2] */
            seq_t second = cache[idx2];

            cache[i] = first + second;
        }
        cached_len = n;
    }

    return cache[n];
}
